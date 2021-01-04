#include <mutex>
#include <memory>
#include <iostream>
//
using namespace std;

/*
 * exam 01 common-singleton
 * exam 02 inner-class-singleton
 * */

//#define CREATE_FIRST

/********************** EXAMPLE 001 *******************/
#ifdef CREATE_FIRST
class Singleton {
  private : /* method */
    Singleton () {};
    virtual ~Singleton () {};

  private : /* member */
    static Singleton* instance;

  public : /* method */
    static Singleton* getInstance ();

    void print ();
};
/* static */
Singleton* Singleton::instance = new Singleton ();

/* static */
Singleton* Singleton::getInstance () {
  return instance;
}

void Singleton::print () {
  cout<<__FILE__<<"\t"<<__func__<<"\t"<<__LINE__<<endl;
}
#else
class Singleton {
  private : /* method */
    Singleton () {};
    virtual ~Singleton () {};

  private : /* member */
    static mutex mlock;
    static Singleton* volatile instance;

  public : /* method */
    static Singleton* volatile getInstance ();

    void print ();
};
/* static */
Singleton* volatile Singleton::instance = NULL;
mutex Singleton::mlock;

/* static */
Singleton* volatile Singleton::getInstance () {
  if (NULL == instance) {
    mlock.lock ();
    if (NULL == instance) {
        instance = static_cast<Singleton *>(new Singleton () );
    }
    mlock.unlock ();
  }
  return instance;
}

void Singleton::print () {
  cout<<__FILE__<<"\t"<<__func__<<"\t"<<__LINE__<<endl;
}
#endif

/********************** EXAMPLE 002 *******************/
class IClient {
  public :
    virtual void print () = 0;
};

class InnerSingleton {
  private :
    InnerSingleton () {};
    virtual ~InnerSingleton () {};

  public :
    void print () {
      cout<<__FILE__<<"\t"<<__func__<<"\t"<<__LINE__<<endl;
    }

    class Client : public IClient {
      private :
        static InnerSingleton* volatile c_instance;
        static mutex mlock;

        Client () {};
        virtual ~Client () {};

      public :
        static InnerSingleton* volatile create () {
          if (NULL != c_instance) {
            mlock.lock ();
            if (NULL != c_instance) {
              c_instance = static_cast<InnerSingleton *> (new InnerSingleton () );
            }
            mlock.unlock ();
          }
          return c_instance;
        }
    }; // Client;
};

typedef InnerSingleton::Client Client;
InnerSingleton* volatile Client::c_instance = 0;
mutex Client::mlock;

/********************** EXAMPLE 003 *******************/

int main ()
{
  // Exam 001
  Singleton::getInstance ()->print ();
  
  // Exam 002
  //Client::create ()->print ();
  
  return 0;
}

