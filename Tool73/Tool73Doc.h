
// Tool73Doc.h : CTool73Doc Ŭ������ �������̽�
//


#pragma once


class CTool73Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CTool73Doc();
	DECLARE_DYNCREATE(CTool73Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CTool73Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


