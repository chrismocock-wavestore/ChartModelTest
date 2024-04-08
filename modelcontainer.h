#ifndef MODELCONTAINER_H
#define MODELCONTAINER_H

#include <QAbstractListModel>
#include <QtQmlIntegration>

#include <unordered_map>

#include "model.h"

class ModelContainer : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ModelContainer)
    QML_ELEMENT

    Q_PROPERTY(qreal minValue READ getMinValue NOTIFY minValueChanged)
    Q_PROPERTY(qreal maxValue READ getMaxValue NOTIFY maxValueChanged)

public:
    ModelContainer();
    ~ModelContainer();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void init();

    Q_INVOKABLE const Model* getCurrentModel() const;

    Q_INVOKABLE void setCurrentModel(const QString& key);

    qreal getMinValue() const;

    qreal getMaxValue() const;

    void emplace(ModelKey key, ModelUPtr&& model);

signals:
    void modelChanged();
    void minValueChanged();
    void maxValueChanged();

private:
    std::unique_ptr<Model> empty_model_;
    std::unordered_map<ModelKey, ModelUPtr> models_;
    std::vector<ModelKey> keys_;
    ModelKey current_model_key_;
    Model* current_model_{};
};

#endif // MODELCONTAINER_H
