
#include "test_autonumbers.hpp"
#include "tags/tagdataroles.hpp"

namespace Test {
    using namespace Coquillo::TagEditor::NumberStrategy;

    void AutoNumbers::itemOrderStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = ItemOrderStrategy().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[2] == 3);
        QVERIFY(result[3] == 4);
        QVERIFY(result[4] == 5);
    }

    void AutoNumbers::fileNumberStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = FileNumberStrategy().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[2] == 11);
        QVERIFY(result[3] == 5);
        QVERIFY(result[4] == 8);
    };

    void AutoNumbers::originalNumbersStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = PreserveOriginalNumbers().suggestions(items);

        QVERIFY(result[0] == 1);
        QVERIFY(result[1] == 2);
        QVERIFY(result[4] == 8);
        QVERIFY(result.contains(2) == false);
        QVERIFY(result.contains(3) == false);
    }

    void AutoNumbers::trackNumberingTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        /*
         * NOTE: AutoNumbers does not specifically require to use the right column for indices,
         * but this is done in order to simplify result validation below.
         */
        const QModelIndexList items = {
            model->index(0, Coquillo::Tags::NumberField),
            model->index(1, Coquillo::Tags::NumberField),
            model->index(2, Coquillo::Tags::NumberField),
            model->index(3, Coquillo::Tags::NumberField),
            model->index(4, Coquillo::Tags::NumberField),
        };

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        QVERIFY(items[0].data() == 1);
        QVERIFY(items[1].data() == 2);
        QVERIFY(items[2].data() == 11);
        QVERIFY(items[3].data() == 5);
        QVERIFY(items[4].data() == 8);
    }

    void AutoNumbers::complexTrackNumberingTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(complexTestData());

        /*
         * NOTE: AutoNumbers does not specifically require to use the right column for indices,
         * but this is done in order to simplify result validation below.
         */
        const QModelIndexList items = {
            model->index(0, Coquillo::Tags::NumberField),
            model->index(1, Coquillo::Tags::NumberField),
            model->index(2, Coquillo::Tags::NumberField),
            model->index(3, Coquillo::Tags::NumberField),
            model->index(4, Coquillo::Tags::NumberField),
            model->index(5, Coquillo::Tags::NumberField),
            model->index(6, Coquillo::Tags::NumberField),
            model->index(7, Coquillo::Tags::NumberField),
        };

        Coquillo::TagEditor::AutoNumbers::autoNumberItems(model, items);

        QVERIFY(items[0].data() == 1);
        QVERIFY(items[1].data() == 4);
        QVERIFY(items[2].data() == 3);
        QVERIFY(items[3].data() == 2);
        QVERIFY(items[4].data() == 11);
        QVERIFY(items[5].data() == 5);
        QVERIFY(items[6].data() == 8);
        QVERIFY(items[7].data() == 12);
    }

    void AutoNumbers::fileDiscNumberStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(testData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
        };

        const auto result = FileNumberStrategy(FileNumberStrategy::DiscNumberMode).suggestions(items);

        QVERIFY(result.contains(0) == false);
        QVERIFY(result[1] == 1);
        QVERIFY(result[2] == 2);
        QVERIFY(result[3] == 2);
        QVERIFY(result[4] == 1);
    }

    void AutoNumbers::discNumberPathNameStrategyTest() {
        auto * model = new TestTagsModel;
        model->initializeTestData(complexTestData());

        const QModelIndexList items = {
            model->index(0, 0),
            model->index(1, 0),
            model->index(2, 0),
            model->index(3, 0),
            model->index(4, 0),
            model->index(5, 0),
            model->index(6, 0),
            model->index(7, 0),
        };

        const auto result = DiscNumberPathNameStrategy().suggestions(items);

        QVERIFY(result.contains(0) == false);
        QVERIFY(result.contains(3) == false);
        QVERIFY(result.contains(4) == false);
        QVERIFY(result.contains(5) == false);
        QVERIFY(result.contains(6) == false);
        QVERIFY(result[1] == 1);
        QVERIFY(result[2] == 1);
        QVERIFY(result[7] == 2);
    }

    void AutoNumbers::trackCountFromMetaDataTest() {
        /*
         * NOTE: Test data needs to be grouped by folder in order to properly emulate the process
         * that AutoNumbers utility executes.
         */

        auto * model = new TestTagsModel;
        model->initializeTestData(complexTestData());

        model->setData(model->index(0, Coquillo::Tags::NumberField), 1);
        model->setData(model->index(0, Coquillo::Tags::DiscNumberField), 1);

        model->setData(model->index(1, Coquillo::Tags::NumberField), 4);
        model->setData(model->index(1, Coquillo::Tags::DiscNumberField), 1);

        model->setData(model->index(2, Coquillo::Tags::NumberField), 3);
        model->setData(model->index(2, Coquillo::Tags::DiscNumberField), 1);

        model->setData(model->index(3, Coquillo::Tags::NumberField), 2);
        model->setData(model->index(3, Coquillo::Tags::DiscNumberField), 1);

        model->setData(model->index(4, Coquillo::Tags::NumberField), 11);
        model->setData(model->index(4, Coquillo::Tags::DiscNumberField), 2);

        model->setData(model->index(5, Coquillo::Tags::NumberField), 5);
        model->setData(model->index(5, Coquillo::Tags::DiscNumberField), 2);

        model->setData(model->index(6, Coquillo::Tags::NumberField), 8);
        model->setData(model->index(6, Coquillo::Tags::DiscNumberField), 1);

        model->setData(model->index(7, Coquillo::Tags::NumberField), 12);
        model->setData(model->index(7, Coquillo::Tags::DiscNumberField), 2);

        /*
         * Group 1: folder 'foobar'
         * NOTE: Contains two discs in the same folder!
         */
        auto result = TrackCountFromMetaData().suggestions({
            model->index(0, 0),
            model->index(3, 0),
            model->index(4, 0),
            model->index(5, 0),
            model->index(6, 0),
        });

        QVERIFY(result[0] == 8);
        QVERIFY(result[1] == 8);
        QVERIFY(result[2] == 11);
        QVERIFY(result[3] == 11);
        QVERIFY(result[4] == 8);

        /*
         * Group 2: folder 'another/CD1'
         */
        result = TrackCountFromMetaData().suggestions({
            model->index(1, 0),
            model->index(2, 0),
        });

        QVERIFY(result[0] == 4);
        QVERIFY(result[1] == 4);

        /*
         * Group 3: folder 'another/Disc 2'
         */
        result = TrackCountFromMetaData().suggestions({
            model->index(7, 0),
        });

        QVERIFY(result[0] == 12);
    }

    QList<QVariantHash> AutoNumbers::testData() const {
        QList<QVariantHash> items;

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/01. file1.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "My First Track"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 1},
                        // {"TRCK[b]", 10},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/102_file2.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Song Number Two"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 2},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/211. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Outro"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/205. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Composition #5"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.append(QVariantHash({
            {
                {"path", "/test/music/foobar/108 - file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Octa"},
                        {"TALB", "Dev Album"},
                        {"TCON", "Techno"},
                        {"TRCK", 8},
                    })}
                })}
            }
        }));

        return items;
    }

    QList<QVariantHash> AutoNumbers::complexTestData() const {
        QList<QVariantHash> items = testData();

        items.insert(1, QVariantHash({
            {
                {"path", "/test/music/another/CD1/04 - file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #4"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.insert(2, QVariantHash({
            {
                {"path", "/test/music/another/CD1/3. file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #3"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        items.insert(7, QVariantHash({
            {
                {"path", "/test/music/another/Disc 2/12_file.mp3"},
                {"primary", "id3v2"},
                {"tags", QVariantHash({
                    {"id3v2", QVariantHash({
                        {"TPE1", "Foobar"},
                        {"TIT2", "Demo #12"},
                        {"TALB", "Second Album"},
                        {"TCON", "Techno"},
                    })}
                })}
            }
        }));

        return items;
    }

    void TestTagsModel::initializeTestData(const QList<QVariantHash> & items) {
        store().add(items);
    }
}

QTEST_MAIN(Test::AutoNumbers)
