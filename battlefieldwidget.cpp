#include "battlefieldwidget.h"
#include "ui_battlefieldwidget.h"
#include "ui_endgamewidget.h"

BattlefieldWidget::BattlefieldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BattlefieldWidget)
{
    ui->setupUi(this);
    view_.reset(new BattlefieldView(":/resources/back.jpg"));
    scene_.reset( new QGraphicsScene(QRectF(view_->rect())));
    view_->setScene(scene_.data());
    ui->verticalLayout->addWidget(view_.data());
    connect(view_.data(), &BattlefieldView::mouseClicked, this, &BattlefieldWidget::slotAddMine);

    int rows = 16;
    int columns = 11;
    pointMatrix_.reset(new PointMatrix(rows, columns));
    startGame();
}

BattlefieldWidget::~BattlefieldWidget()
{
    delete ui;
}

void BattlefieldWidget::slotAddMine(QPoint mousePoint)
{
    if(ship_->isSailing())
        return;
    QPoint centerObj = pointMatrix_->getPlaceToInsert(mousePoint);
    if(centerObj == QPoint(-1, -1))
        return; //the point is specified as an undefined value
    ObjectOfGame* item = new ObjectOfGame(":/resources/mine.png");
    item->setPos(centerObj);
    scene_->addItem(item);
    beginningMoveShip();
}

void BattlefieldWidget::startGame()
{
    int rows = pointMatrix_->rowCount();
    int columns = pointMatrix_->columnCount();
    scene_->clear();
    pointMatrix_->refreshMatrix();
    ship_ = new Ship(":/resources/ship.png");
    ship_->setPos(pointMatrix_->getPlaceToInsert(rows/2, columns/2)); //set on centers
    curShipIndex_.reset(new Index(rows/2, columns/2));
    scene_->addItem(ship_);
    this->setEnabled(true);
}

void BattlefieldWidget::beginningMoveShip()
{
    QVector<Index> way = routeCalculation(); //calculation of the way
    if(way.size() > 1)
    {
        ship_->moveTo(pointMatrix_->getSwapPlaceToInsert(*curShipIndex_.data(), way.last())); // begin move ship
        curShipIndex_->column = way.last().column;
        curShipIndex_->row = way.last().row;
    }
    else if(way.size() == 0)
    {
        this->setEnabled(false);
        endGame_.reset(new EndGameWidget(true));
        connect(endGame_.data(), &EndGameWidget::restartGame, this, &BattlefieldWidget::startGame);
        endGame_->show();
    }
    else if(way.size() == 1)
    {
        this->setEnabled(false);
        ship_->moveTo(pointMatrix_->getPlaceToInsert(way.last().row, way.last().column));
        endGame_.reset(new EndGameWidget(false));
        connect(endGame_.data(), &EndGameWidget::restartGame, this, &BattlefieldWidget::startGame);
        endGame_->show();
    }
}

QVector<Index> BattlefieldWidget::routeCalculation()
{
    QVector<QVector<int>> &weightPoints = pointMatrix_->weightPoints_;
    for(int indexRow = 0; indexRow < weightPoints.size(); ++indexRow)
        for(int indexColumn = 0; indexColumn < weightPoints[indexRow].size(); ++indexColumn)
        {
            if(pointMatrix_->isHaveObject(indexRow, indexColumn))
                weightPoints[indexRow][indexColumn] = 9999; //by default, we set a large number
            else
                weightPoints[indexRow][indexColumn] = -1;
        }


    QSharedPointer<Index> curShipIndex = curShipIndex_;
    QVector<Index> oddMotionOptions = {Index(-1,-1), Index(-1,0), Index(1,-1), Index(1,0)};
    QVector<Index> evenMotionOptions = {Index(-1,0), Index(-1,1), Index(1,0), Index(1,1)};

    auto findingWays = [&weightPoints, &curShipIndex, &oddMotionOptions, &evenMotionOptions](){

        QVector<Index> wave;
        QVector<Index> oldWave;
        oldWave.push_back(Index(curShipIndex->row,curShipIndex->column));
        int nstep = 0;
        weightPoints[curShipIndex->row][curShipIndex->column] = nstep;
        while(oldWave.size() > 0)
        {
            ++nstep;
            wave.clear();
            for(auto it = oldWave.begin(); it != oldWave.end(); ++it)
            {
                QVector<Index> motionOptions;
                if((it->row + 1) % 2 == 0)  //odd and even column
                    motionOptions = evenMotionOptions;
                else
                    motionOptions = oddMotionOptions;
                for(int i = 0; i < motionOptions.size(); ++i)
                {
                    int nRow = it->row + motionOptions[i].row;
                    int nColumn = it->column + motionOptions[i].column;
                    if(nRow < 0 || nColumn < 0 || nRow == weightPoints.size())
                        continue;
                    if(nColumn == weightPoints[nRow].size())
                        continue;
                    if(weightPoints[nRow][nColumn] == -1)
                    {
                        wave.push_back(Index(nRow, nColumn));
                        weightPoints[nRow][nColumn] = nstep;
                    }

                }
            }
            oldWave = wave;
        }
    };
    Index endPoint;
    int lengthWay = 9999;




    findingWays();


    //find way with the lowest weight (shortest path)
    for(int curRow = 0; curRow < weightPoints.size(); curRow++)
        for(int curColumn = 0; curColumn < weightPoints[curRow].size(); curColumn++)
        {
            if(curRow == 0 || curRow ==  weightPoints.size() - 1
                    || ((curRow + 1) % 2 != 0 && (curColumn == 0 || curColumn == weightPoints[curRow].size() - 1)))
                if(weightPoints[curRow][curColumn] < lengthWay && weightPoints[curRow][curColumn] != -1)
                {
                    endPoint.column = curColumn;
                    endPoint.row = curRow;
                    lengthWay = weightPoints[curRow][curColumn];
                }
        }

    if(lengthWay == 9999) // if no way, out
      return QVector<Index>();

    //building route
    QVector<Index> shortWay;
    shortWay.push_back(endPoint);
    while (weightPoints[endPoint.row][endPoint.column] !=0)
    {
        QVector<Index> motionOptions;
        if((endPoint.row + 1) % 2 == 0)
            motionOptions = evenMotionOptions;
        else
            motionOptions = oddMotionOptions;
        for(int i = 0; i < motionOptions.size(); i++)
        {
            int nRow = endPoint.row + motionOptions[i].row;
            int nColumn = endPoint.column + motionOptions[i].column;
            if(nRow < 0 || nColumn < 0 || nRow == weightPoints.size())
                continue;
            if(nColumn == weightPoints[nRow].size())
                continue;
            if(weightPoints[endPoint.row][endPoint.column] - weightPoints[nRow][nColumn] == 1)
            {
                endPoint.row = nRow;
                endPoint.column = nColumn;
                shortWay.push_back(endPoint);
                break;
            }
        }
    }
    shortWay.pop_back();
    return shortWay;

//    return QVector<Index>();
}
