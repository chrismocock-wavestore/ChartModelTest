#include "model.h"

// TODO - adjust range to something sensible
std::pair<qreal, qreal> adjustMinMaxRange(qreal min, qreal max) {
    if (min == std::numeric_limits<qreal>::max()) {
        min = std::max(0.0, max - 100);
    }

    if (max == std::numeric_limits<qreal>::min()) {
        max = min + 100;
    }

    return {min, max};
}

struct Model::Private {
    Private(Model* parent);

    void updateRangeInfoForKey();

    Model* q_{};
    ModelKey key_;
    std::vector<QDateTime> date_times_;
    std::vector<QVariant> data_;
    RangeInfo range_info_;
};

Model::Private::Private(Model* parent) : q_(parent) {}

void Model::Private::updateRangeInfoForKey() {
    // check for system systus
    if (key_ == u"Status") {
        range_info_.stats_type_ = StatsType::ENUM_SYSTEM_STATUS;
        range_info_.lower_range_type_ = FIXED;
        range_info_.upper_range_type_ = FIXED;
        return;
    }

    // check for disk status
    if (key_.startsWith(u"Disk_") && key_.length() == 6) {
        if (key_.at(5).isLetter() && key_.at(5).isUpper()) {
            range_info_.stats_type_ = StatsType::ENUM_DISK_STATUS;
            range_info_.lower_range_type_ = FIXED;
            range_info_.upper_range_type_ = FIXED;
            return;
        }
    }

    // check for camera status
    if (key_.startsWith('C') && key_.length() > 1) {
        QString cam_id = key_;
        cam_id.remove(0, 1);
        bool numeric = true;
        for (auto digit : cam_id) {
            if (!digit.isDigit()) {
                numeric = false;
                break;
            }
        }
        if (numeric) {
            range_info_.stats_type_ = StatsType::ENUM_CAMERA_STATUS;
            range_info_.lower_range_type_ = FIXED;
            range_info_.upper_range_type_ = FIXED;
            return;
        }
    }

    if (key_ == u"CPU%") {
        // this can go above 100% on multi-core systems apparently
        range_info_.stats_type_ = StatsType::FLOAT;
        range_info_.lower_range_type_ = FIXED;
        range_info_.upper_range_type_ = AUTO;
        range_info_.lower_range_value_ = 0.0;
        range_info_.upper_range_value_ = 10.0; // initially
        return;
    }

}

/******************************************* Public *******************************************/

Model::Model() : QAbstractTableModel(), d_(std::make_unique<Private>(this)) {
    qDebug() << "Model" << this;
}

Model::~Model() {
    qDebug() << "~Model()" << this;
}

int Model::rowCount(const QModelIndex &parent ) const {
    return d_->date_times_.size();
}

int Model::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant Model::data(const QModelIndex &index, int role) const {
    if (index.column() == 0) {
        return d_->date_times_.at(index.row());
    } else if (index.column() == 1) {
        return d_->data_.at(index.row());
    }
    return QVariant();
}

void Model::setModelKey(const ModelKey& key) const {
    d_->key_ = key;
    d_->updateRangeInfoForKey();
}

const ModelKey& Model::modelKey() const {
    return d_->key_;
}

void Model::addRows(std::vector<std::pair<QDateTime, qreal>> rows) {
    beginResetModel();
    for (const auto& row : rows) {
        d_->date_times_.emplace_back(row.first);
        qreal value = row.second;

        if (!d_->range_info_.min_data_value_ || !d_->range_info_.max_data_value_) {
            d_->range_info_.min_data_value_ = value;
            d_->range_info_.max_data_value_ = value;
        } else {
            d_->range_info_.min_data_value_ = std::min(value, d_->range_info_.min_data_value_.value());
            d_->range_info_.max_data_value_ = std::max(value, d_->range_info_.max_data_value_.value());
        }

        d_->data_.emplace_back(value);
    }

    std::pair<qreal, qreal> min_max = adjustMinMaxRange(d_->range_info_.min_data_value_.value(), d_->range_info_.max_data_value_.value());
    d_->range_info_.min_data_value_ = min_max.first;
    d_->range_info_.max_data_value_ = min_max.second;

    endResetModel();
}

qreal Model::minValue() const {
    if (!d_->range_info_.min_data_value_.has_value()) {
        return 0.0;
    }
    return d_->range_info_.min_data_value_.value();
}

qreal Model::maxValue() const {
    if (!d_->range_info_.max_data_value_.has_value()) {
        return 100.0;
    }
    return d_->range_info_.max_data_value_.value();
}
