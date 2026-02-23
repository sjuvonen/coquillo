#ifndef COQUILLO_FINDER_H
#define COQUILLO_FINDER_H

#include "media.h"
#include <future>
#include <list>
#include <vector>

namespace coquillo::finder {
typedef std::vector<std::string> media_paths;
typedef std::vector<void *> media_items;

struct find_job {
    find_job();

    std::future<media_paths> future;
    std::atomic<bool> aborted;
    std::atomic<bool> completed;
};

struct read_job {
    read_job();

    const std::list<Coquillo::Media> flush();

    std::mutex mutex;
    std::vector<std::future<void>> futures;
    std::atomic<int> jobsFinished;
    std::atomic<bool> aborted;
    std::atomic<bool> completed;
    std::atomic<int> progress;
    std::list<Coquillo::Media> media;
};

find_job *find_files(const std::string &directory, bool recursive);
read_job *read_files(const media_paths &files);

}; // namespace coquillo::finder
#endif