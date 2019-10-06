#include "pointmatrix.h"

bool operator < (QPoint &p1, QPoint &p2)
{
    if((p1.x() < p2.x()) && (p1.y() < p2.y()))
        return true;
    else return false;
}

bool operator > (QPoint &p1, QPoint &p2)
{
    if((p1.x() > p2.x()) && (p1.y() > p2.y()))
        return true;
    else return false;
}

QPoint PointMatrix::getPlaceToInsert(QPoint posePoint)
{
    if(poseOfPoints_.isEmpty())
        return QPoint(-1, -1);

    Index indexPose(0,0);
    bool isFindObject = false;

    auto findSimilar = [&posePoint, &indexPose, &isFindObject](QPoint it)
    {
        if(isFindObject)
            return;
        QPoint area(15,15); // the area in which objects will be add
        auto sumPoints = it + area;
        auto diffPoints = it - area;
        if(sumPoints > posePoint &&  diffPoints < posePoint)
        {
            isFindObject = true;
            return;
        }
        else
            indexPose.column++;
    };

    for(; indexPose.row < poseOfPoints_.size(); indexPose.row++)
    {
        indexPose.column = 0;
        for(auto it = poseOfPoints_[indexPose.row].begin(); it < poseOfPoints_[indexPose.row].end(); it++)
        {
            if(!isFindObject)
                findSimilar(*it);
            else break;
        }
        if(isFindObject)
            break;
    }

    if(!isHaveObject(indexPose.row,indexPose.column) && isFindObject)
    {
        isHaveMinePoints_[indexPose.row][indexPose.column] = true;
        return poseOfPoints_[indexPose.row][indexPose.column];
    }

    return QPoint(-1, -1);
}

QPoint PointMatrix::getPlaceToInsert(int indexRow, int indexColumn)
{
    if(!isHaveObject(indexRow, indexColumn) && isValid(indexRow, indexColumn))
    {
        isHaveMinePoints_[indexRow][indexColumn] = true;
        return poseOfPoints_[indexRow][indexColumn];
    }
    else
        return QPoint(-1, -1);
}

QPoint PointMatrix::getSwapPlaceToInsert(Index curIndex, Index replaceIndex)
{
    isHaveMinePoints_[curIndex.row][curIndex.column] = false;  // clear previous position
    return getPlaceToInsert(replaceIndex.row, replaceIndex.column);
}

void PointMatrix::fillMatrixPose()
{
    Index indexPose(0,0);
    QPoint firstPoint;

    // experimental values
    float differentPointX = 61.8;
    float differentPointY = 31.4;
    firstPoint.setX(190);
    firstPoint.setY(155);
    //------

    for(indexPose.row = 0; indexPose.row < rowsNumber_; ++indexPose.row)
    {
        poseOfPoints_.push_back(QVector<QPoint>()); // add row
        int currPoseY = firstPoint.y() + std::round(differentPointY * indexPose.row);
        for(indexPose.column = 0; indexPose.column < columnsNumber_; ++indexPose.column)
        {
            int currPoseX;
            if(indexPose.row % 2 == 0)
                currPoseX = firstPoint.x() + std::round(differentPointX * indexPose.column);
            else
                currPoseX = firstPoint.x() + std::round(differentPointX * indexPose.column) + differentPointX / 2;

            QPoint point;
            point.setX(currPoseX);
            point.setY(currPoseY);
            poseOfPoints_[indexPose.row].push_back(point);
        }
    }
}

void PointMatrix::fillMatrixWeightAndIsHave()
{
    int indexRow;

    for(indexRow = 0; indexRow < rowsNumber_; ++indexRow)
    {
        weightPoints_.push_back(QVector<int>()); // add row
        isHaveMinePoints_.push_back(QVector<bool>()); // add row

        weightPoints_[indexRow].fill(99999, columnsNumber_); //by default, we set a large number
        isHaveMinePoints_[indexRow].fill(false, columnsNumber_);
    }
}

bool PointMatrix::isHaveObject(int indexRow, int indexColumn)
{
    if (isValid(indexRow, indexColumn))
        return isHaveMinePoints_[indexRow][indexColumn];
    return false;
}

void PointMatrix::refreshMatrix()
{
    for(int indexRow = 0; indexRow < rowsNumber_; ++indexRow)
        isHaveMinePoints_[indexRow].fill(false, columnsNumber_);
}

bool PointMatrix::isValid(int indexRow, int indexColumn)
{
    if (!isHaveMinePoints_.isEmpty() && indexRow < isHaveMinePoints_.size())
        if(!isHaveMinePoints_[indexRow].isEmpty() && indexColumn < isHaveMinePoints_[indexRow].size())
            return true;
    return false;
}
