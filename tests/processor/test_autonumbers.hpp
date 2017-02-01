#ifndef TEST_AUTONUMBERS_HPP
#define TEST_AUTONUMBERS_HPP

#include <QtTest/QtTest>

#include "tags/tagsmodel.hpp"
#include "tageditor/autonumbers.hpp"

namespace Test {
  class AutoNumbers : public QObject {
    Q_OBJECT

    Coquillo::Tags::TagsModel * _model;

    private slots:
      void initTestCase();
      void basicText();
      void extendedTest();
  };

  class TestTagsModel : public Coquillo::Tags::TagsModel {
    Q_OBJECT

    public:
      TestTagsModel(QObject * parent = nullptr)
      : Coquillo::Tags::TagsModel(nullptr, parent) { }

      void initializeTestData();

  };
}

#endif
