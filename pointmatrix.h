#ifndef POINTMATRIX_H
#define POINTMATRIX_H
#include <QPoint>
#include <QVector>


struct Index
{
    Index(int row, int column) {this->row = row; this->column = column; }
    Index(){}
public:
    int column;
    int row;
};

class PointMatrix
{

public:
    PointMatrix(int rowsNumber, int columnsNumber) :
        rowsNumber_(rowsNumber),
        columnsNumber_(columnsNumber)
    {
        fillMatrixPose();
        fillMatrixWeightAndIsHave();
    }
    QPoint getPlaceToInsert(QPoint posePoint);
    QPoint getPlaceToInsert(int indexRow, int indexColumn);
    QPoint getSwapPlaceToInsert(Index curIndex, Index replaceIndex);
    QVector<QVector<int>> weightPoints_;
    bool isValid(int indexRow, int indexColumn);
    bool isHaveObject(int indexRow, int indexColumn);
    void refreshMatrix();
    int rowCount()  const {return rowsNumber_;}
    int columnCount()  const {return columnsNumber_;}
private:
    void fillMatrixPose();
    void fillMatrixWeightAndIsHave();
    QVector<QVector<QPoint>> poseOfPoints_;
    QVector<QVector<bool>> isHaveMinePoints_;
    int rowsNumber_;
    int columnsNumber_;
};

#endif // POINTMATRIX_H
