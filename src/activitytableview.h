#ifndef __ACTIVITYTABLEVIEW_H
#define __ACTIVITYTABLEVIEW_H

#include <QTableView>
#include <QDate>
#include "activitytablemodel.h"

class ActivityTableView : public QTableView
{
  Q_OBJECT

  public:
    ActivityTableView(QWidget *parent = 0);

    void setDate(const QDate &date);
    ActivityTableModel *model() const;
    void setModel(ActivityTableModel *model);

    QSize minimumSizeHint() const;

  protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void columnsInserted(const QModelIndex &parent, int start, int end);
    void modelReset();

  private:
    void resizeHeightIfFixed();
};

#endif