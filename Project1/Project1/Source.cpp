///******************************************************************************************************
//** Задача «Постачальник – обробник - споживач».
//** Реалізуйте задачу по обробці інформації через список з хх буферів обміну
//** (наприклад, циклічний список),
//** коли:
//** перший потік (постачальник) наповнює буфери обміну даними;
//** другий потік (обробник) перекодовує «великі» літери тексту на маленькі;
//** третій потік (споживач) записує оброблені записи даних у вихідний файл.
//** Коректно реалізована задача не призведе до зміни тексту у вихідному файлі (по довжині, по значенню).
//******************************************************************************************************/
//
//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <queue>
//#include <condition_variable>
//#include <chrono>
//#include <algorithm>
//#include <fstream>
//using namespace std::literals;
//
//int max_buffers = 3;
//constexpr int buffer_size = 20;
//std::string buffer[buffer_size];
//std::mutex mutex;
//std::condition_variable provider_can_write;
//std::condition_variable consumer_can_read;
//std::condition_variable hander_can_modify;
//int data_left_to_produce = buffer_size;
//int data_left_to_consume = 0;
//int data_left_to_modify = 0;
//
//void producer()
//{
//	char letter = 'a';
//	int i{ 0 };
//	int num{ 0 };
//	while (num < max_buffers) {
//		std::string data = std::string{ letter }+std::string{ "\n" };
//		std::unique_lock<std::mutex> lc{ mutex };
//		provider_can_write.wait(lc, [=](){
//			return data_left_to_produce > 0;
//		});
//		--data_left_to_produce;
//		buffer[i] = data;
//		if (data_left_to_modify < buffer_size)
//			++data_left_to_modify;
//		hander_can_modify.notify_one();
//		lc.unlock();
//		++i;
//		if (!(i%buffer_size)) {
//			i = 0;
//			++num;
//		}
//		++letter;
//		if (letter > 'z')
//			letter = 'a';
//		std::this_thread::sleep_for(100ms); // emulate hard work
//	}
//}
//
//void handler()
//{
//	int num{ 0 };
//	int i{ 0 };
//	while (num < max_buffers) {
//		std::unique_lock<std::mutex> lc{ mutex };
//		hander_can_modify.wait(lc, [=](){
//			return data_left_to_modify > 0;
//		});
//		--data_left_to_modify;
//		std::transform(buffer[i].begin(), buffer[i].end(), buffer[i].begin(), ::toupper);
//		if (data_left_to_consume < buffer_size)
//			++data_left_to_consume;
//		std::this_thread::sleep_for(1000ms); // emulate hard work
//		consumer_can_read.notify_one();
//		lc.unlock();
//		++i;
//		if (!(i%buffer_size)) {
//			i = 0;
//			++num;
//		}
//	}
//}
//
//void consumer()
//{
//	int num{ 0 };
//	int i{ 0 };
//	std::ofstream fout("consumer.txt", std::ofstream::out | std::ofstream::trunc);
//	while (num < max_buffers) {
//		std::unique_lock<std::mutex> lc{ mutex };
//		consumer_can_read.wait(lc, [=](){return data_left_to_consume >0; });
//		--data_left_to_consume;
//		auto m = buffer[i];
//		++data_left_to_produce;
//		provider_can_write.notify_one();
//		lc.unlock();
//		++i;
//		if (!(i%buffer_size)) {
//			i = 0;
//			++num;
//		}
//		std::cout << m;
//		fout << m;
//		std::flush(std::cout);
//		std::this_thread::sleep_for(500ms); // emulate hard work
//	}
//	fout.close();
//}
//
//int main()
//{
//	std::thread producer_thread{ producer };
//	std::thread handler_thread{ handler };
//	std::thread consumer_thread{ consumer };
//
//	producer_thread.join();
//	handler_thread.join();
//	consumer_thread.join();
//
//	return 0;
//}
