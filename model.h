#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <optional>
#include <QAbstractTableModel>
#include <QDateTime>
#include <QString>

using ModelKey = QString;

class Model;
using ModelUPtr = std::unique_ptr<Model>;

//! Represents statistics for a single data entry, e.g. CPU fan speed or CPU temperature
class Model : public QAbstractTableModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Model)

public:
    enum class StatsType {
        FLOAT,
        INT,
        ENUM_SYSTEM_STATUS,
        ENUM_DISK_STATUS,
        ENUM_CAMERA_STATUS
    };

    enum RangeType {
        AUTO,
        FIXED
    };

    struct RangeInfo {
        StatsType stats_type_{StatsType::FLOAT};
        RangeType lower_range_type_{RangeType::AUTO};
        RangeType upper_range_type_{RangeType::AUTO};

        std::optional<qreal> min_data_value_{};
        std::optional<qreal> max_data_value_{};
        qreal lower_range_value_{std::numeric_limits<qreal>::max()};
        qreal upper_range_value_{std::numeric_limits<qreal>::min()};
    };

    Model();
    ~Model();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setModelKey(const ModelKey& key) const;

    const ModelKey& modelKey() const;

    void addRows(std::vector<std::pair<QDateTime, qreal>> rows);

    qreal minValue() const;

    qreal maxValue() const;

private:
    struct Private;
    std::unique_ptr<Private> d_;
};

#endif // MODEL_H
