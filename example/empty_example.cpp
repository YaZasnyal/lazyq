// #include <chrono>
// #include <iostream>
// #include <string_view>
// #include <thread>

// #include <lazyq/lazyq.hpp>

// struct DataType
// {
//   DataType(const DataType&) = default;
//   DataType(DataType&&) = default;
//   DataType& operator=(const DataType&) = default;
//   DataType& operator=(DataType&&) = default;
//   DataType(std::string_view sv)
//       : value_view(sv)
//   {
//   }

//   void to_owned()
//   {
//     owned_value = std::string(value_view.data(), value_view.size());
//     value_view = std::string_view();
//   }

//   std::string_view value()
//   {
//     if (owned_value) {
//       return *owned_value;
//     }
//     return value_view;
//   }

//   std::string_view value_view;
//   std::optional<std::string> owned_value;
// };

// class Processor
// {
// public:
//   using Data = DataType;

//   bool finished() const { return false; }

//   std::chrono::steady_clock::time_point execute(Data&& data)
//   {
//     std::cout << std::this_thread::get_id() << ": " << data.value() << "; "
//               << data.owned_value.has_value() << "\n";
//     return std::chrono::steady_clock::time_point::max();
//   }
// };

auto main() -> int
{
  // lazyq::executor<std::size_t, Processor, std::hash<std::size_t>> q(10);

  // std::jthread jt = std::jthread(
  //     [&]()
  //     {
  //       for (int i = 0; i < 10; ++i) {
  //         q.execute(1, {"Hello world from another thread"});
  //         std::this_thread::sleep_for(std::chrono::milliseconds(1));
  //       }
  //     });

  // for (int i = 0; i < 10; ++i) {
  //   q.execute(1, {"Hello world from main thread"});
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
  // }

  return 0;
}
