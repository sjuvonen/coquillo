
#include "crawler/crawler.hpp"
#include "modeljobwatcher.hpp"
#include "tagwriter.hpp"

namespace Coquillo {
    namespace Tags {
        ModelJobWatcher::ModelJobWatcher(QObject * parent)
        : QObject(parent) {

        }

        void ModelJobWatcher::add(Crawler::Crawler * crawler) {
            if (crawler) {
                _crawlers << crawler;
            }
        }

        void ModelJobWatcher::add(Writer * writer) {
            if (writer) {
                _writers << writer;
            }
        }

        void ModelJobWatcher::abortAll() {
            for (int i = 0; i < _crawlers.size(); i++) {
                if (_crawlers[i]) {
                    _crawlers[i]->abort();
                }
            }

            for (int i = 0; i < _writers.size(); i++) {
                if (_writers[i]) {
                    _writers[i]->abort();
                }
            }
        }
    }
}
