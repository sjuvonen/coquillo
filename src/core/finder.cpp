#include "finder.h"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <thread>

namespace coquillo::finder {
void read_files_job(media_paths::const_iterator &&cbegin, media_paths::const_iterator &&cend,
                    read_job *job);

find_job::find_job() : aborted(false), completed(false) {}
read_job::read_job() : aborted(false), completed(false), jobsFinished(0), progress(0) {}

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

                std::cout << "found: " << it.path() << std::endl;
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
    auto job_count = 1;
    size_t batch_size = std::ceil(files.size() / (float)job_count);

    auto job = new read_job();
    std::vector<std::future<void>> futures;

    for (int i = 0; i < job_count; i++) {
        auto ibegin = batch_size * i;
        auto iend = std::min(ibegin + batch_size, files.size());
        auto begin = files.cbegin() + ibegin;
        media_paths::const_iterator end = files.begin() + iend;

        std::cout << "Reader thread for slice " << ibegin << " " << iend << std::endl;

        auto future =
            std::async(std::launch::async, read_files_job, std::move(begin), std::move(end), job);

        futures.push_back(std::move(future));
    }

    job->futures = std::move(futures);

    return job;
}

void read_files_job(media_paths::const_iterator &&cbegin, media_paths::const_iterator &&cend,
                    read_job *job) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::lock_guard<std::mutex> guard(job->mutex);

    job->jobsFinished++;
    int foo = 0;

    for (auto it = cbegin; it != cend; it++) {
        // TagLib::FileRef file_ref(it->c_str);

        std::cout << "open: " << *it << std::endl;

        foo++;
    }

    std::cout << "    thread read " << foo << " entries" << std::endl;

    if (job->jobsFinished == job->futures.size()) {
        job->completed.store(true);
    }
}

} // namespace coquillo::finder