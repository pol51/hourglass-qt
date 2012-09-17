#ifndef __ACTIVITYTABLEMODEL_H
#define __ACTIVITYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include "activity.h"

class ActivityTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    ActivityTableModel(QObject *parent = 0);
    ActivityTableModel(QDate date, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  public slots:
    void fetchActivities();

  private slots:
    void activityDurationChanged();

  private:
    static const QString s_timeSeparator;
    QDate m_date;
    QDateTime m_lastFetchedAt;
    QList<Activity *> m_activities;
};

#endif