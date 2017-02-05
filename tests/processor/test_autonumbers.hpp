#ifndef TEST_AUTONUMBERS_HPP
#define TEST_AUTONUMBERS_HPP

#include <QtTest/QtTest>

#include "tags/tagsmodel.hpp"
#include "tageditor/autonumbers.hpp"

namespace Test {
  class AutoNumbers : public QObject {
    Q_OBJECT

    /**
     * Generates a set of items all from the same album.
     */
    QList<QVariantHash> testData() const;

    /**
     * Generates a set of items that have different folders and albums.
     */
    QList<QVariantHash> complexTestData() const;

    private slots:
      /**
       * Validates ItemOrderStrategy.
       */
      void itemOrderStrategyTest();

      /**
       * Validates FileNumberStrategy.
       */
      void fileNumberStrategyTest();

      /**
       * Validates PreserveOriginalNumbers strategy.
       */
      void originalNumbersStrategyTest();

      /**
       * Validates numbering of items that consists from a single album.
       */
      void trackNumberingTest();

      /**
       * Validates numbering of items from multiple albums at once.
       */
      void complexTrackNumberingTest();

      /**
       * Validates FileNumberStrategy for reading disc numbers.
       */
      void fileDiscNumberStrategyTest();

      /**
       * Validates DiscNumberPathNameStrategy
       */
      void discNumberPathNameStrategyTest();
  };

  /**
   * Mock-up that allows to manually set the data contained in the model.
   */
  class TestTagsModel : public Coquillo::Tags::TagsModel {
    Q_OBJECT

    public:
      TestTagsModel(QObject * parent = nullptr)
      : Coquillo::Tags::TagsModel(nullptr, parent) { }

      void initializeTestData(const QList<QVariantHash> & items);
  };
}

#endif
