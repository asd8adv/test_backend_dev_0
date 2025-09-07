
// 1 обращения к Client::player, потенциальная ошибка player может быть не инициализирован.
// 2 обращения к переменным членам класса через this->, это не является обязательным но может быть приемлемо если так приято в кодстайле. вместе с тем в методе Client::send обращение напрямую. 
// 3 Client::login возможна передача net_id и net_type в конструктор LoginData, если такой конструктор есть.
// 4 получения данных из server::Packet с помощью магических чисел.

#include "Client.h"

#include "Log.h"
#include "LoginData.h"

Client::Client(IO *io):
	io(io)
{
	this->requests = std::make_shared<Requests>(this);
}

Client::~Client() = default;

auto Client::get_ip() const -> IP
{
	return this->io->get_ip();
}

void Client::disconnect() const
{
	this->io->stop();
}

void Client::send(const server::Packet &packet) const
{
	io->write(packet);
}

void Client::on_event(const ClientEvent &event)
{
	// Пример обработки событий клиента
	switch (event.get_type())
	{
		case ClientEvent::Type::DISCONNECT:
			this->disconnect();
			break;
		default:
			logger->warning("Unhandled event type {}", event.get_type());
	}
}

void Client::on_packet(const server::Packet &packet)
{
	switch (packet.get_type())
	{
		case server::PacketType::PARAMS_SET:
			this->params_set(packet);
			break;
		case server::PacketType::BUY:
			this->buy(packet);
			break;
		case server::PacketType::LOGIN:
			this->login(packet);
			break;
		default:
			logger->warning("Unhandled packet type {}", packet.get_type());
			break;
	}
}

void Client::params_set(const server::Packet &packet) const
{
	string params = packet.S(0);
	if (params.empty())
		return;

	this->player->params->set(params);

	logger->info("Client {} params set", this->player->id);
}

void Client::buy(const server::Packet &packet)
{
	// здесь мы должны гарантировать что player инициализирован, так же player->balance, player->inventory. проверять это внутри функции не рекомендуется из-за потери производительности на "горячих" участках.
	// ***
	// избавляемся от магических переменных, лучше это сделать для всего класса например вынести в namespace или отдельный заголовочный файл.
	constexpr int ITEM_ID_IDX = 0;
	uint32_t item_id = packet.I(ITEM_ID_IDX);

	// избавляемся от обращений this в соответствии с кодстайлом.
	if (!player->balance->can_afford(item_id)) {
		logger->warning("Client {} can't afford item {}", player->id, item_id);
		return;
	}

	player->balance->deduct(item_id);
	player->inventory->add(item_id);

	logger->info("Client {} bought item {}", player->id, item_id);
}

void Client::login(const client::Packet *packet)
{
	uint64_t net_id = packet->L(0);
	uint8_t net_type = packet->B(1);
	const string &auth_key = packet->S(3);

	if (net_type >= NetType::MAX_TYPE)
	{
		logger->warning("Player net_id {} sent wrong net_type {}", net_id, net_type);
		this->send(server::Login(server::Login::Status::FAILED));
		return;
	}

	if (!Api::check_auth(net_id, net_type, auth_key))
	{
		logger->debug("Player net_id {}, net_type {} sent wrong auth_key '{}'", net_id, net_type, auth_key);
		this->send(server::Login(server::Login::Status::FAILED));
		return;
	}

	LoginData data;
	data.net_id = net_id;
	data.net_type = net_type;

	this->requests->add(&data, [&](const vector<Player*> &loaded) -> void
	{
		this->login_do(loaded[0], &data);
	});
}