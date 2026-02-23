#include "finder.h"
#include "src/core/media.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <future>
#include <iostream>
#include <iterator>
#include <queue>
#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <utility>

namespace coquillo::finder {
void read_files_job(const media_paths &&files, read_job *job);

find_job::find_job() : aborted(false), completed(false) {}
read_job::read_job() : aborted(false), completed(false), jobsFinished(0), progress(0) {}

const std::list<Coquillo::Media> read_job::flush() {
    std::lock_guard<std::mutex> guard(mutex);

    std::list<Coquillo::Media> batch;
    std::swap(media, batch);

    return batch;
};

media_paths read_directory(const std::string &directory, bool recursive, find_job *job) {
    std::queue<std::string> directories;
    std::vector<std::string> files;

    directories.push(directory);

    while (!directories.empty()) {
        if (job->aborted) {
            std::cout << "Finding media was aborted!" << std::endl;
            std::terminate();
        }

        const std::filesystem::path path{directories.front()};
        const std::filesystem::directory_iterator iterator{path};

        for (auto const it : iterator) {
            if (recursive && it.is_directory()) {
                directories.push(it.path());
            } else if (it.is_regular_file()) {
                files.push_back(it.path());
            }
        }

        directories.pop();
    }

    job->completed.store(true);

    return files;
};

find_job *find_files(const std::string &directory, bool recursive) {
    auto job = new find_job();
    auto future = std::async(std::launch::async, read_directory, directory, recursive, job);
    job->future = std::move(future);

    return job;
}

read_job *read_files(const media_paths &files) {
    auto job_count = 4;
    size_t batch_size = std::ceil(files.size() / (float)job_count);

    auto job = new read_job();
    std::vector<std::future<void>> futures;

    for (int i = 0; i < job_count; i++) {
        auto ibegin = batch_size * i;
        auto iend = std::min(ibegin + batch_size, files.size());
        auto begin = files.cbegin() + ibegin;
        media_paths::const_iterator end = files.cbegin() + iend;

        std::cout << "Reader thread for slice " << ibegin << " " << iend << std::endl;

        media_paths slice;

        std::copy(begin, end, std::back_inserter(slice));

        auto future = std::async(std::launch::async, read_files_job, std::move(slice), job);

        futures.push_back(std::move(future));
    }

    job->futures = std::move(futures);

    return job;
}

void read_files_job(const media_paths &&files, read_job *job) {
    for (const auto path : files) {
        const TagLib::FileRef file_ref(path.c_str());

        if (!file_ref.isNull()) {
            Coquillo::Media media = std::move(Coquillo::Media::fromFileRef(file_ref));

            std::lock_guard<std::mutex> guard(job->mutex);
            job->media.push_back(std::move(media));
            job->progress++;
        } else {
            std::lock_guard<std::mutex> guard(job->mutex);
            job->progress++;
        }
    }

    if (++job->jobsFinished == job->futures.size()) {
        job->completed.store(true);
    }
}

} // namespace coquillo::finder