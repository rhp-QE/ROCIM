#include <net/ILongConnection.h>
#include <queue>

namespace roc::net {

class LongConnectionImpl : public ILongConnection<LongConnectionImpl>, public std::enable_shared_from_this<LongConnectionImpl> {
public:
    LongConnectionImpl(boost::asio::io_context& io_context, const LongConnectionConfig& config);

    void send_impl(const std::vector<char>& data);
    void connect_impl();
    void disconnect_impl();
    void set_connect_callback_impl(ConnectCallback callback);
    void set_receive_callback_impl(ReceiveCallback callback);

private:
    void do_connect();
    void do_read();
    void do_write();
    void schedule_reconnect();
    void handle_disconnect(const boost::system::error_code& ec);

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::steady_timer reconnect_timer_;
    LongConnectionConfig config_;
    std::array<char, 1024> read_buffer_;
    std::queue<std::vector<char>> write_queue_;
    bool connected_ = false;

    std::shared_ptr<ILongConnection<LongConnectionImpl>> create_client(
        boost::asio::io_context& io_context,
        const LongConnectionConfig& config
    );

protected:
    ReceiveCallback receive_callback_;
    ConnectCallback connect_callback_;

};

} /// namespace roc::net