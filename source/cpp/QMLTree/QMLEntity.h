
#pragma once

#include "../qtplus_global.h"

//-------------------------------------------------------------------------------------------------

// Qt
#include <QObject>
#include <QVariant>
#include <QTextStream>
#include <QPoint>

// Library
#include "../CDumpable.h"
#include "../CXMLNodable.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class QMLTreeContext;

//-------------------------------------------------------------------------------------------------

//! Defines a base QML item
class QTPLUSSHARED_EXPORT QMLEntity : public QObject, public CDumpable, public CXMLNodable
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    QMLEntity(const QPoint& pPosition);

    //! Constructor with QVariant
    QMLEntity(const QPoint& pPosition, const QVariant& value);

    //! Destructor
    virtual ~QMLEntity();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void setValue(const QVariant& value);

    //!
    virtual void setPosition(const QPoint& point);

    //!
    virtual void setOrigin(QMLEntity* pItem);

    //!
    void setIsParenthesized(bool bValue);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QPoint position() const;

    //!
    QMLEntity* origin() const;

    //!
    bool isParenthesized() const;

    //!
    virtual QVariant value() const;

    //!
    virtual QString toString() const;

    //! Returns all members
    virtual QMap<QString, QMLEntity*> members();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Virtual control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void solveOrigins(QMLTreeContext* pContext);

    //!
    virtual QMap<QString, QMLEntity*> getDeclaredSymbols();

    //!
    virtual QMLEntity* findSymbolDeclaration(const QString& sName, bool bDescending);

    //!
    virtual void toQML(QTextStream& stream, QMLTreeContext* pContext, QMLEntity* pParent = nullptr, int iIdent = 0);

    //-------------------------------------------------------------------------------------------------
    // Overridden methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual CXMLNode toXMLNode(CXMLNodableContext* pContext, CXMLNodable* pParent) Q_DECL_OVERRIDE;

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QVariant    m_vValue;
    QPoint      m_pPosition;
    QMLEntity*  m_pOrigin;
    bool        m_bIsParenthesized;
};
