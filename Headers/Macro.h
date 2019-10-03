#ifndef _MACRO_H_
#define _MACRO_H_

#define NO_COPY(ClassName)		\
private:						\
ClassName(const ClassName&)	;	\
ClassName& operator =(const ClassName&);	\


#define DECLARE_SINGLETON(ClassName)		\
	NO_COPY(ClassName)					\
private:								\
	static ClassName*	m_pInstance;	\
public:									\
	static ClassName* GetInstance(void)	\
	{									\
		if(m_pInstance == NULL)			\
			m_pInstance = new ClassName;\
		return m_pInstance;				\
	}									\
	void Destroy(void)					\
	{									\
		if(m_pInstance)					\
		{								\
			delete m_pInstance;			\
			m_pInstance = NULL;			\
		}								\
	}									\
private:								\
ClassName(void);						\
public:									\
	~ClassName(void);					\


#define IMPLEMENT_SINGLETON(ClassName)		\
	ClassName* ClassName::m_pInstance = NULL;\
	ClassName::ClassName(void)				 \
	{										 \
	}										 \
	ClassName::~ClassName(void)				 \
	{										 \
		ClassName::Release();				 \
	}										 \

#define ERROR_TEXT(Text)		\
	MessageBox(g_hWnd, Text, L"Error!", MB_OK);				\


#define  ERROR_MSG(Message)						\
	MessageBox(g_hWnd, Message, L"SystemError Message!", NULL) \

#endif	//_MACRO_H_