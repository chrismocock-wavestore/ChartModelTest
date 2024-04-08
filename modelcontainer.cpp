#include "modelcontainer.h"

#include <QDateTime>

ModelContainer::ModelContainer() : QAbstractListModel() {
    empty_model_ = std::make_unique<Model>();
    current_model_ = empty_model_.get();
    std::srand( ( unsigned int )std::time( nullptr ) );
}

ModelContainer::~ModelContainer() {
    qDebug() << "~ModelContainer()";
}

int ModelContainer::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return models_.size();
}

QVariant ModelContainer::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        return keys_.at(index.row());
    }
    return QVariant();
}

void ModelContainer::init() {
    // Generate some test models

    const QDate date(2023, 3, 15);
    const int model_count = 10;
    char model_letter = 'A';

    const int min_val  = 0;
    const int max_val = 10;

    for (int i = 1; i <= model_count; ++i) {
        auto model = std::make_unique<Model>();
        const ModelKey model_key(QString("Model ") + QChar(model_letter));
        model->setModelKey(model_key);
        std::vector<std::pair<QDateTime, qreal>> rows {
                                                      { QDateTime(date, QTime(9, 0, 0)), std::rand() },
            { QDateTime(date, QTime(9, 1, 0)), min_val + std::rand() % (max_val - min_val) },
            { QDateTime(date, QTime(9, 2, 0)), min_val + std::rand() % (max_val - min_val) },
            { QDateTime(date, QTime(9, 3, 0)), min_val + std::rand() % (max_val - min_val) },
            { QDateTime(date, QTime(9, 4, 0)), min_val + std::rand() % (max_val - min_val) },
            { QDateTime(date, QTime(9, 5, 0)), min_val + std::rand() % (max_val - min_val) }};
        model->addRows(rows);
        emplace(model_key, std::move(model));
        model_letter++;
    }
}

const Model* ModelContainer::getCurrentModel() const {
    return current_model_;
}

void ModelContainer::setCurrentModel(const QString& key) {
    beginResetModel();
    current_model_key_ = key;
    if (models_.find(key) != models_.end()) {
        current_model_ = models_.at(key).get();
    }
    endResetModel();
    emit minValueChanged();
    emit maxValueChanged();
}

qreal ModelContainer::getMinValue() const {
    if (!current_model_) {
        return 0.0;
    }
    return current_model_->minValue();
}

qreal ModelContainer::getMaxValue() const {
    if (!current_model_) {
        return 100.0;
    }
    return current_model_->maxValue();
}

void ModelContainer::emplace(ModelKey key, ModelUPtr&& model) {
    models_.emplace(key, std::move(model));
    beginInsertRows(QModelIndex(), keys_.size(), keys_.size());
    keys_.emplace_back(key);
    endInsertRows();
}
