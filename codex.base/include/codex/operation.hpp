/**
 */
#ifndef __codex_operation_h__
#define __codex_operation_h__

#include <memory>

namespace codex {

  template < class T >
  class operation;

  template < class R , class ...Args>
  class operation< R ( Args... ) > {
  public:
    typedef R (*handler_type)( operation* op , Args... );
  public:
    operation( handler_type handler )
      : _handler( handler )
    {
    }

    ~operation( void ) {
    }

    R operator()( Args&&... arg ) {
      //cassert( _execute != nullptr );
      if ( _handler )
        return _handler( this , std::forward< Args >(arg)... );
      return R();
    }

    operation* next( void ) const {
      return _next;
    }
    operation* next( operation* op ) {
      std::swap( _next , op );
      return op;
    }
  private:
    handler_type _handler;
    operation* _next;
  public:
    template < class Handler >
    static operation* wrap( Handler&& handler ){
      class _op : public operation {
        public:
          _op( Handler&& h ) 
            : operation( &_op::execute_impl )
            , _handler( std::forward<Handler>( h ) ){
            }

          ~_op( void ) {
          }
          static R execute_impl( operation* op ,  Args&&... arg ) {
            _op* ptr = static_cast< _op* > (op);
            Handler handler( std::move( ptr->_handler ));
            delete ptr;
            handler( std::forward< Args >(arg)... );  
          }
        private:
          Handler _handler;
      };
      return new _op( std::forward< Handler >(handler)); 
    }
  };
}

#endif
