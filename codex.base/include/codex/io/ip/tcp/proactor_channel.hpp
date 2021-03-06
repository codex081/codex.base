#ifndef __codex_io_ip_tcp_proactor_channel_h__
#define __codex_io_ip_tcp_proactor_channel_h__

#include <codex/codex.hpp>
#include <codex/buffer/packetizer.hpp>
#include <codex/io/ip/socket_ops.hpp>
#include <codex/proactor/iocp.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class event_handler;
  class channel_builder;

  class proactor_channel {
  public:
    proactor_channel( void );
    ~proactor_channel( void );

    codex::loop& loop( void );

    int bind( io::ip::socket_ops<>::socket_type fd );

    void close( void );
    bool closed( void );

    void write( codex::buffer::shared_blk blk );

    int add_ref( void );
    int release( void );

    template < class Option >
    bool set_option( Option& opt ) {
      return codex::io::ip::socket_ops<>::setsockopt( _fd , opt );
    }
  public:
    void reset( void );
    void set_builder( channel_builder* builder );
    void set_loop( codex::loop* loop );
    void set_packetizer( const std::shared_ptr< codex::buffer::packetizer >& packetizer );
    void set_handler( const std::shared_ptr< event_handler >& handler );
  private:
    static void handle_read0( void* ctx 
      , const std::error_code& ec 
      , const int io_bytes );
    static void handle_write0(void* ctx
      , const std::error_code& ec
      , const int io_bytes);
    void handle_read(const std::error_code& ec
      , const int io_bytes);
    void handle_write(const std::error_code& ec
      , const int io_bytes);

    void handle_error( const std::error_code& ec );
    void write0( codex::buffer::shared_blk blk );

    void do_read(void);
    void do_write(void);
  private:
    io::ip::socket_ops<>::socket_type _fd;
    codex::proactor::iocp::handler _read_handler;
    codex::proactor::iocp::handler _write_handler;
    codex::threading::atomic<int> _ref_count;
    codex::loop* _loop;
    std::shared_ptr< event_handler > _handler;
    std::shared_ptr< codex::buffer::packetizer > _packetizer;
    std::deque< codex::buffer::shared_blk > _write_packets;
    channel_builder* _builder;
  };

  typedef std::shared_ptr< proactor_channel > channel_ptr;

}}}}

#endif
