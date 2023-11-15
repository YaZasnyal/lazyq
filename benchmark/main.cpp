#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

#include <benchmark/benchmark.h>
#include <lazyq/lazyq.hpp>

struct DataType
{
  DataType(const DataType&) = default;
  DataType(DataType&&) = default;
  DataType& operator=(const DataType&) = default;
  DataType& operator=(DataType&&) = default;
  DataType(std::string_view sv)
  //: value_view(sv)
  {
  }

  void to_owned()
  {
    // if(owned_value)
    //     return;
    // owned_value = std::string(value_view.data(), value_view.size());
    // value_view = std::string_view();
  }

  std::string_view value()
  {
    // if (owned_value) {
    //   return *owned_value;
    // }
    // return value_view;
    return "";
  }

  // std::string_view value_view;
  // std::optional<std::string> owned_value;
};

template<class Locker = std::mutex>
class Processor
    : public lazyq::lazyq<Processor<Locker>,
                          lazyq::owned<std::string_view>,
                          lazyq::queue_type<lazyq::owned<std::string_view>, Locker>>
{
public:
  using MessageType = DataType;

  void process_message(lazyq::owned<std::string_view>&& data) noexcept final
  {
    // std::cout << std::this_thread::get_id() << ": " << data.value() << "; "
    //           << data.owned_value.has_value() << "\n";
  }
};

struct null_mutex
{
  void lock() noexcept {}
  void unlock() noexcept {}
  bool try_lock() noexcept { return true; }
};

static void push_null_mutex(benchmark::State& s)
{
  static Processor<null_mutex> q;

  for (auto _ : s) {
    q.post_message({"Hello world from main thread"});
  }
}
BENCHMARK(push_null_mutex);

// static void push_bench(benchmark::State& s)
// {
//   static Processor<boost::detail::spinlock> q;

//   for (auto _ : s) {
//     q.post_message({"Hello world from main thread"});
//   }
// }
// BENCHMARK(push_bench)->ThreadRange(1, 8);

static void push_bench_mutex(benchmark::State& s)
{
  static Processor q;

  for (auto _ : s) {
    q.post_message({"Hello world from main thread"});
  }
}
BENCHMARK(push_bench_mutex)->ThreadRange(1, 8);

BENCHMARK_MAIN();