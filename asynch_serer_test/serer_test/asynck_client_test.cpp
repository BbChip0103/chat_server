// 헤더 파일 추가
#include "stdafx.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

// boost의 tcp 라이브러리 사용
using boost::asio::ip::tcp;

// 버퍼의 크기를 설정하는 enum 변수
enum myEnum
{
	eBuffSize = 128,
};

boost::mutex io_mutex;

void receive_handle(tcp::socket * _p_socket) {
	// 읽어올 데이터를 저장하기 위해 배열 선언
	boost::array<char, eBuffSize> buf;
	boost::system::error_code error;

	// read_some 함수를 이용해 서버로부터 메시지를 전달 받음
	size_t len = _p_socket->read_some(boost::asio::buffer(buf), error);

	// 해당 서버는 전송 이후에 접속이 끊어지므로 error:eof 리턴
	if (error == boost::asio::error::eof) {
		//break;
		exit(0);
	}
	else if (error)
		throw boost::system::system_error(error);

	//boost::mutex::scoped_lock lock(io_mutex);

	// 입력 받은 데이터를 cout를 이용해 출력
	std::cout << "받은 메세지 : ";
	std::cout.write(buf.data(), len);
	std::cout << std::endl;
}

void send_handle(tcp::socket * _p_socket) {
	// 보낼 데이터를 저장하기 위해 배열 선언
	boost::array<char, eBuffSize> buf_send;
	boost::system::error_code error;

	// 클라이언트에서 서버로 보낼 메세지를 입력받음
	std::cout << "보낼 메세지 : ";
	//boost::mutex::scoped_lock lock(io_mutex);
	std::cin.getline(buf_send.data(), eBuffSize);
	int len_str = std::cin.gcount() - 1;

	// 입력받은 문자열을 송신
	int len_send = boost::asio::write(*_p_socket
		, boost::asio::buffer(buf_send, len_str)
		, error);
	if (len_send > 0) {
		std::cout << "보내진 메세지 : ";
		std::cout.write(buf_send.data(), len_str);
		std::cout << std::endl;
	}
	else {
		std::cout << "error!" << std::endl;
	}
}

int _tmain(int argc, char* argv[])
{
	try
	{
		// io_service 객체를 생성하여 지정된 서버를 resolver로 처리함
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		//tcp::resolver::query query(IP, "5000");
		tcp::resolver::query query("ec2-35-164-63-73.us-west-2.compute.amazonaws.com"
									, "5000"
									, tcp::resolver::query::numeric_service);
		tcp::resolver::iterator myIterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// 접속할 소켓을 생성
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;

		// resolver로 처리된 endpoint를 이용해 접속을 시도함
		while (error && myIterator != end)
		{
			socket.close();
			socket.connect(*myIterator++, error);
		}

		// 오류가 발생한 경우
		if (error)
			throw boost::system::system_error(error);

		// 접속에 성공한 경우
		std::cout << "서버와 클라이언트 동기식 1대1 채팅 시작" << std::endl << std::endl;
		while (true)
		{
			boost::thread thrd_rec(boost::bind(&receive_handle, &socket));
			boost::thread thrd_snd(boost::bind(&send_handle, &socket));

			thrd_rec.join();
			thrd_snd.join();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	// 프로그램 종료
	system("pause");
	return 0;
}