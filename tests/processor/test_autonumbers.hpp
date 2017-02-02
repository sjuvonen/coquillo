#ifndef TEST_AUTONUMBERS_HPP
#define TEST_AUTONUMBERS_HPP

#include <QtTest/QtTest>

#include "tags/tagsmodel.hpp"
#include "tageditor/autonumbers.hpp"

namespace Test {
  class AutoNumbers : public QObject {
    Q_OBJECT

    QList<QVariantHash> testData() const;

    private slots:
      void itemOrderStrategyTest();
      void fileNumberStrategyTest();
      void originalNumbersStrategyTest();

      void trackNumberingTest();
  };

  class TestTagsModel : public Coquillo::Tags::TagsModel {
    Q_OBJECT

    public:
      TestTagsModel(QObject * parent = nullptr)
      : Coquillo::Tags::TagsModel(nullptr, parent) { }

      void initializeTestData(const QList<QVariantHash> & items);
  };
}

#endif
