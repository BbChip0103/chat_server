// ��� ���� �߰�
#include "stdafx.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

// boost�� tcp ���̺귯�� ���
using boost::asio::ip::tcp;

// ������ ũ�⸦ �����ϴ� enum ����
enum myEnum
{
	eBuffSize = 128,
};

boost::mutex io_mutex;

void receive_handle(tcp::socket * _p_socket) {
	// �о�� �����͸� �����ϱ� ���� �迭 ����
	boost::array<char, eBuffSize> buf;
	boost::system::error_code error;

	// read_some �Լ��� �̿��� �����κ��� �޽����� ���� ����
	size_t len = _p_socket->read_some(boost::asio::buffer(buf), error);

	// �ش� ������ ���� ���Ŀ� ������ �������Ƿ� error:eof ����
	if (error == boost::asio::error::eof) {
		//break;
		exit(0);
	}
	else if (error)
		throw boost::system::system_error(error);

	boost::mutex::scoped_lock lock(io_mutex);

	// �Է� ���� �����͸� cout�� �̿��� ���
	std::cout << "���� �޼��� : ";
	std::cout.write(buf.data(), len);
	std::cout << std::endl;
}

void send_handle(tcp::socket * _p_socket) {
	// ���� �����͸� �����ϱ� ���� �迭 ����
	boost::array<char, eBuffSize> buf_send;
	boost::system::error_code error;

	// Ŭ���̾�Ʈ���� ������ ���� �޼����� �Է¹���
	std::cout << "���� �޼��� : ";
	boost::mutex::scoped_lock lock(io_mutex);
	std::cin.getline(buf_send.data(), eBuffSize);
	int len_str = std::cin.gcount() - 1;

	// �Է¹��� ���ڿ��� �۽�
	int len_send = boost::asio::write(*_p_socket
		, boost::asio::buffer(buf_send, len_str)
		, error);
	if (len_send > 0) {
		std::cout << "������ �޼��� : ";
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
		// io_service ��ü�� �����Ͽ� ������ ������ resolver�� ó����
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		//tcp::resolver::query query(IP, "5000");
		tcp::resolver::query query("ec2-35-164-63-73.us-west-2.compute.amazonaws.com"
									, "5000"
									, tcp::resolver::query::numeric_service);
		tcp::resolver::iterator myIterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// ������ ������ ����
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;

		// resolver�� ó���� endpoint�� �̿��� ������ �õ���
		while (error && myIterator != end)
		{
			socket.close();
			socket.connect(*myIterator++, error);
		}

		// ������ �߻��� ���
		if (error)
			throw boost::system::system_error(error);

		// ���ӿ� ������ ���
		std::cout << "������ Ŭ���̾�Ʈ ����� 1��1 ä�� ����" << std::endl << std::endl;
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

	// ���α׷� ����
	system("pause");
	return 0;
}