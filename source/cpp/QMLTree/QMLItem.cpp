
// Qt
#include <QDebug>

// Application
#include "QMLItem.h"
#include "QMLPropertyDeclaration.h"
#include "QMLPropertyAssignment.h"

//-------------------------------------------------------------------------------------------------

QMLItem::QMLItem(const QPoint& pPosition, QMLEntity* pName)
    : QMLComplexEntity(pPosition, pName)
    , m_bIsSingleton(false)
{
    m_iUsageCount = 1;
}

//-------------------------------------------------------------------------------------------------

QMLItem::~QMLItem()
{
}

//-------------------------------------------------------------------------------------------------

QString QMLItem::id() const
{
    return m_sID;
}

//-------------------------------------------------------------------------------------------------

QMap<QString, QMLEntity*> QMLItem::unusedProperties()
{
    QMap<QString, QMLEntity*> mReturnValue;

    if (m_bIsSingleton == false)
    {
        foreach (QString sKey, m_mPropertyList.keys())
        {
            if (m_mPropertyList[sKey]->usageCount() == 0)
            {
                mReturnValue[sKey] = m_mPropertyList[sKey];
            }
        }
    }

    return mReturnValue;
}

//-------------------------------------------------------------------------------------------------

void QMLItem::markAsSingleton()
{
    m_bIsSingleton = true;
}

//-------------------------------------------------------------------------------------------------

/*!
    Finds all symbols in the entity. \br\br
    \a pContext is the context of this entity. \br
*/
void QMLItem::solveSymbols(QMLTreeContext* pContext)
{
    QMLComplexEntity::solveSymbols(pContext);

    foreach (QMLEntity* pEntity, m_vContents)
    {
        QMLPropertyDeclaration* pDeclaration = dynamic_cast<QMLPropertyDeclaration*>(pEntity);
        QMLPropertyAssignment* pAssignment = dynamic_cast<QMLPropertyAssignment*>(pEntity);

        if (pDeclaration != nullptr && pAssignment == nullptr)
        {
            if (pDeclaration->name() != nullptr)
            {
                QString sName = pDeclaration->name()->toString();

                m_mPropertyList[sName] = pDeclaration->name();
            }
        }
        else
        {
            pEntity->solveReferences(pContext);
        }
    }

    m_mPropertyList["id"] = this;

    foreach (QMLEntity* pEntity, m_vContents)
    {
        QMLPropertyAssignment* pAssignment = dynamic_cast<QMLPropertyAssignment*>(pEntity);

        if (pAssignment != nullptr)
        {
            QMLIdentifier* pIdentifier = dynamic_cast<QMLIdentifier*>(pAssignment->name());

            if (pIdentifier != nullptr && pIdentifier->toString() == "id")
            {
                QMLIdentifier* pValue = dynamic_cast<QMLIdentifier*>(pAssignment->content());

                if (pValue != nullptr)
                {
                    m_sID = pValue->toString();
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Finds the origin of the entity. \br\br
    \a pContext is the context of this entity. \br
*/
void QMLItem::solveReferences(QMLTreeContext* pContext)
{
    QMLComplexEntity::solveReferences(pContext);

    foreach (QMLEntity* pEntity, m_vContents)
    {
        pEntity->solveReferences(pContext);
    }
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item named \a sName, for identifier resolution.
*/
QMLEntity* QMLItem::findSymbolDeclaration(const QString& sName)
{
    QStringList lQualifiedName = QMLEntity::qualifiedNameAsList(sName);

    if (lQualifiedName.count() > 0)
    {
        if (lQualifiedName[0] == id())
        {
            if (lQualifiedName.count() > 1)
            {
                lQualifiedName.removeAt(0);
            }
            else
            {
                return this;
            }
        }

        if (lQualifiedName.count() == 1)
        {
            if (m_mPropertyList.contains(lQualifiedName[0]))
            {
                return m_mPropertyList[lQualifiedName[0]];
            }
        }

        return QMLComplexEntity::findSymbolDeclaration(QMLEntity::listAsQualifiedName(lQualifiedName));
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

/*!
    Returns the item named \a sName, for identifier resolution.
*/
QMLEntity* QMLItem::findSymbolDeclarationDescending(QStringList& lQualifiedName)
{
    if (lQualifiedName.count() > 0)
    {
        if (lQualifiedName[0] == id())
        {
            if (lQualifiedName.count() > 1)
            {
                lQualifiedName.removeAt(0);
            }
            else
            {
                return this;
            }
        }

        if (lQualifiedName.count() == 1)
        {
            if (m_mPropertyList.contains(lQualifiedName[0]))
            {
                return m_mPropertyList[lQualifiedName[0]];
            }
        }
    }

    return QMLComplexEntity::findSymbolDeclarationDescending(lQualifiedName);
}

//-------------------------------------------------------------------------------------------------

CXMLNode QMLItem::toXMLNode(CXMLNodableContext* pContext, CXMLNodable* pParent)
{
    CXMLNode xNode = QMLComplexEntity::toXMLNode(pContext, pParent);
    CXMLNode xPropertyList("PropertyList");

    foreach (QString sKey, m_mPropertyList.keys())
    {
        CXMLNode xProperty("Property");
        xProperty.attributes()["Name"] = sKey;
        xPropertyList << xProperty;
    }

    xNode.nodes() << xPropertyList;

    return xNode;
}
