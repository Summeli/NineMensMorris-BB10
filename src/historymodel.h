#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>
#include <QString>

class HistoryElement
{
public:
    HistoryElement(int player, int from, int to, qlonglong board, int phase);

    QString title() const;
    QString subtitle() const;
    QString sourceIcon() const;
    QString section() const;

private:
    int m_player;
    int m_from;
    int m_to;
    qlonglong m_board;
    int m_phase;
};

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count)
public:
    enum HistoryRoles {
        TitleRole = Qt::UserRole + 1,
        SubtitleRole,
        IconSourceRole,
        SectionRole
    };

    explicit HistoryModel(QObject *parent = 0);

    void addMove(const HistoryElement &move);
    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    int count() const;
    Q_INVOKABLE QVariantMap get(int index) const;

private:
    QList<HistoryElement> m_moves;

signals:

public slots:

};

#endif // HISTORYMODEL_H
