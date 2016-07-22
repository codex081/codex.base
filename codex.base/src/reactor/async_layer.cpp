#include <codex/codex.hpp>
#include <codex/slist.hpp>
#include <codex/operation.hpp>
#include <codex/loop.hpp>
#include <codex/reactor/reactor.hpp>
#include <codex/reactor/async_layer.hpp>
#include <codex/io/operation.hpp>
#include <codex/io/ip/socket_ops.hpp>

namespace codex { namespace reactor {

  struct async_layer::descriptor {
    codex::loop& loop;
    int fd;
    poll_handler handler;
    codex::slist< codex::io::operation > ops[2];
  
    descriptor( codex::loop& l )
      : loop(l)
      , fd ( -1 )
      , handler( &async_layer::descriptor::handle_events )
    {
    }

    ~descriptor( void ) {

    }

    void handle_events( const int poll_ev ) {
      static const codex::reactor::poll_events ev[2] = {
        codex::reactor::poll_events::pollin ,
        codex::reactor::poll_events::pollout 
      };
      int remain_ev = 0;
      for ( int i = 0 ; i < 2 ; ++i ) {
        if ( poll_ev & ev[i] ) {
          io::operation* op = ops[i].head();
          while( op ) {
            //if ( !(*op)( layer )) 
            //  break;
            ops[i].remove_head();
            (*op)();
            op = ops[i].head();
          }
        }
        if ( ops[i].head() ){
          remain_ev |= ev[i];
        }
      }
      if ( handler.events() != remain_ev ) {
        handler.events(remain_ev);
        loop.engine().implementation().bind( fd , &handler );
      }
    }

    static void handle_events( poll_handler* h , const int poll_ev ) {
      descriptor* desc = codex::container_of( h , &descriptor::handler );
      desc->handle_events( poll_ev );
    }
  };

  async_layer::descriptor_type async_layer::wrap( int native_fd ) {
    return std::make_shared< descriptor >(this,native_fd);
  }

}}
