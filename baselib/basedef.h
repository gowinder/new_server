#ifndef __BASEDEF_HEADER__
#define __BASEDEF_HEADER__

#include <time.h>
#include <ctype.h>
#include <string>
#include <stdio.h>
#include <assert.h>

#include <vector>
#include <map>
#include <queue>
#include <list>
#include <fstream>
#include <sstream>
#include <exception>
using namespace std;

namespace gowinder
{

namespace baselib
{


/// \author �粻Ϣ gowinder@hotmail.com
/// \brief �ַ���
/// \date [4/12/2010]
/// \param pTarget Ŀ�껺�����ַ
/// \param pSource Դ��ַ
/// \param nBufLen Ŀ�껺����Ĵ�С
/// \detail
/// \return
bool SafeCopy(char * pTarget, const char * pSource, int nBufLen);

#define LOGERROR	LogTextFunc
#define LOGTIMEERR	LogTimeFunc
#define SAFE_DELETE(ptr) { if(ptr){	try{ delete ptr; }catch(...){ LogDeleteCatchFunc(LOG_FILE, EM_LOG_HIGH, "SAFE_DELETE()", __FILE__, __LINE__); } ptr = 0; } }
#define SAFE_RELEASE(ptr) { if(ptr){ try{ ptr->Release(); }catch(...){ LogDeleteCatchFunc(LOG_FILE, EM_LOG_HIGH, "SAFE_RELEASE()", __FILE__, __LINE__); } ptr = 0; } }
#define SAFE_RELEASEBYOWNER(ptr) { if(ptr){ try{ ptr->ReleaseByOwner(); }catch(...){ LogDeleteCatchFunc(LOG_FILE, EM_LOG_HIGH, "SAFE_RELEASEBYOWNER()", __FILE__, __LINE__); } ptr = 0; } }

#ifdef WIN32
#pragma warning(disable : 4005)
#endif
#define		ASSERT2(x, y)	(void)( (x) || (LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__), 0) )
#define		ASSERT(x)	(void)( (x) || (LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__), 0) )
#define		ASSERT1P(x, z, z1)	(void)( (x) || (LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1), 0) )
#define		ASSERT2P(x, z, z1, z2)	(void)( (x) || (LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2), 0) )

#define		CHECK2(x, y)	do{ if(!(x)) { LogCheckFunc(LOG_FILE,  y, (#x), __FILE__, __LINE__); return; } }while(0)
#define		CHECKF2(x, y)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__); return 0; } }while(0)
#define		CHECK(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE,  EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return; } }while(0)
#define		CHECKF(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return 0; } }while(0)
#define		CHECKRETURN(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return RETURN_CHECK_FAILED; } }while(0)
#define		CHECKNULL(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return RETURN_NULL_REF; } }while(0)
#define		CHECKSAVE(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return DATABASE_SAVE_FAILED; } }while(0)
#define		CHECKCREATE(x)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return RETURN_CREATE_NEW; } }while(0)
#define		CHECKR(x, y)	do{ if(!(x)) { LogCheckFunc(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__); return y; } }while(0)
#define		CHECKP12(x, y, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE,  y, (#x), __FILE__, __LINE__, z, z1); return; } }while(0)
#define		CHECKP1F2(x, y, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, y, (#x), __FILE__, __LINE__); return 0; } }while(0)
#define		CHECKP1(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE,  EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return; } }while(0)
#define		CHECKP1F(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return 0; } }while(0)
#define		CHECKP1RETURN(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return RETURN_CHECK_FAILED; } }while(0)
#define		CHECKP1NULL(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return RETURN_NULL_REF; } }while(0)
#define		CHECKP1SAVE(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return DATABASE_SAVE_FAILED; } }while(0)
#define		CHECKP1CREATE(x, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return RETURN_CREATE_NEW; } }while(0)
#define		CHECKP1R(x, y, z, z1)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1); return y; } }while(0)

#define		CHECKP22(x, y, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE,  y, (#x), __FILE__, __LINE__, z, z1, z2); return; } }while(0)
#define		CHECKP2F2(x, y, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, y, (#x), __FILE__, __LINE__); return 0; } }while(0)
#define		CHECKP2(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE,  EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return; } }while(0)
#define		CHECKP2F(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return 0; } }while(0)
#define		CHECKP2RETURN(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return RETURN_CHECK_FAILED; } }while(0)
#define		CHECKP2NULL(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return RETURN_NULL_REF; } }while(0)
#define		CHECKP2SAVE(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return DATABASE_SAVE_FAILED; } }while(0)
#define		CHECKP2CREATE(x, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return RETURN_CREATE_NEW; } }while(0)
#define		CHECKP2R(x, y, z, z1, z2)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2); return y; } }while(0)

#define		CHECKP3F(x, z, z1, z2, z3)	do{ if(!(x)) { LogCheckFuncWithParam(LOG_FILE, EM_LOG_NORMAL, (#x), __FILE__, __LINE__, z, z1, z2, z3); return 0; } }while(0)

#define		IF_NOT(x, y)	if( !(x) ? ( LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__),1 ) : 0 )
#define		IF_NOT_(x, y)	if( !(x) ? ( LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__),1 ) : 0 )
#define		IF_OK(x, y)	if( (x) ? 1 : ( LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__),0 ) )
#define		IF_OK_(x, y)	if( (x) ? 1 : ( LogCheckFunc(LOG_FILE, y, (#x), __FILE__, __LINE__),0 ) )


#define		DEBUG_TRY try{
// #ifdef WIN32
// #define		DEBUG_CATCH(x)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x); }
// #define		DEBUG_CATCH2(x,y)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y); }
// #define		DEBUG_CATCH3(x,y,z)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z); }
// #define		DEBUG_CATCH4(x,y,z,u)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z, u);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u); }
//
// #define		DEBUG_CATCHF(x)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x);return false; }
// #define		DEBUG_CATCHF2(x,y)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y);return false; }
// #define		DEBUG_CATCHF3(x,y,z)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z);return false; }
//#define		DEBUG_CATCHF4(x,y,z,u)	}catch(se_translator::exception&ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z, u);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u);return false; }
//#else
/*
#define		DEBUG_CATCH(x)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x); }
#define		DEBUG_CATCH2(x,y)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y); }
#define		DEBUG_CATCH3(x,y,z)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z); }
#define		DEBUG_CATCH4(x,y,z,u)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z, u);} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u); }

#define		DEBUG_CATCHF(x)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x);return false; }
#define		DEBUG_CATCHF2(x,y)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y);return false; }
#define		DEBUG_CATCHF3(x,y,z)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z);return false; }
#define		DEBUG_CATCHF4(x,y,z,u)	}catch(std::exception &ex){::LogCatchFunc(LOG_FILE, ex, EM_LOG_HIGH, x, y, z, u);return false;} catch(...){ ::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u);return false; }
*/

#define		DEBUG_CATCH(x)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x); }
#define		DEBUG_CATCH2(x,y)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y); }
#define		DEBUG_CATCH3(x,y,z)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z); }
#define		DEBUG_CATCH4(x,y,z,u)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u); }

#define		DEBUG_CATCHF(x)	} catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x);return false; }
#define		DEBUG_CATCHF2(x,y)	} catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y);return false; }
#define		DEBUG_CATCHF3(x,y,z)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z);return false; }
#define		DEBUG_CATCHF4(x,y,z,u)	}catch(...){ gowinder::baselib::LogCatchFunc(LOG_FILE, EM_LOG_LOW, x, y, z, u);return false; }


//#endif

}
}

#endif
