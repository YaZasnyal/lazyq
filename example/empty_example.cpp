#include <chrono>
#include <iostream>
#include <optional>
#include <string_view>
#include <thread>

#include <lazyq/lazyq.hpp>

struct DataType
{
  DataType(const DataType&) = default;
  DataType(DataType&&) = default;
  DataType& operator=(const DataType&) = default;
  DataType& operator=(DataType&&) = default;
  DataType(std::string_view sv)
      : value_view(sv)
  {
  }

  void to_owned() noexcept
  {
    owned_value = std::string(value_view.data(), value_view.size());
    value_view = std::string_view();
  }

  std::string_view value()
  {
    if (owned_value) {
      return *owned_value;
    }
    return value_view;
  }

  std::string_view value_view;
  std::optional<std::string> owned_value;
};

class Processor 
  : public lazyq::lazyq_functonal<DataType>
  , public lazyq::lazyq_functonal<lazyq::owned<int>>
{
public:
  using lazyq::lazyq_functonal<DataType>::post_message;
  using lazyq::lazyq_functonal<lazyq::owned<int>>::post_message;
  // void process_message(DataType&& data) noexcept final
  // {
  //   std::cout << std::this_thread::get_id() << ": " << data.value() << "; "
  //             << data.owned_value.has_value() << "\n";
  // }
};

auto main() -> int
{
  Processor q;

  std::jthread jt = std::jthread(
      [&]()
      {
        for (int i = 0; i < 10; ++i) {
          q.post_message(DataType{"Hello world from another thread"}, [&](DataType&& d){
            std::cout << std::this_thread::get_id() << ": " << d.value() << "\n";
          });
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
      });

  for (int i = 0; i < 10; ++i) {
    q.post_message(lazyq::owned<int>{5}, [&](lazyq::owned<int>&& v){
      std::cout << std::this_thread::get_id() << ": " << v.data << "\n";
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}
