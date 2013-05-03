/**
* @author Aldebaran Robotics
* Aldebaran Robotics (c) 2012 All Rights Reserved
*/

#include <alserial/alserial.h>
#include <qi/log.hpp>

#include <qicore-compat/model/choregrapheprojectmodel.hpp>
#include "choregrapheprojectmodel_p.hpp"

namespace qi
{
  ChoregrapheProjectModelPrivate::ChoregrapheProjectModelPrivate(const std::string &dir,
                                                       const std::string &name,
                                                       const std::string &formatVersion,
                                                       BoxInstanceModelPtr rootBox) :
    _dir(dir),
    _name(name),
    _formatVersion(formatVersion),
    _rootBox(rootBox)
  {
  }

  bool ChoregrapheProjectModelPrivate::loadFromFile()
  {
    std::string path = _dir + "/behavior.xar";
    std::string errorMsg;
    boost::shared_ptr<AL::XmlDocument> xmlFile = AL::XmlDocument::loadFromXmlFile(path,
                                                                                 NULL,
                                                                                 &errorMsg);

    if (!xmlFile)
    {
      qiLogError("QICore") << "Failed to open the given file : "
                           << path
                           << std::endl
                           << errorMsg
                           << std::endl;
      return false;
    }

    boost::shared_ptr<const AL::XmlElement> root = xmlFile->root();
    if (root == NULL)
    {
      qiLogError("QICore") << "No root element in the given file : "
                           << path
                           << std::endl;
      return false;
    }

    root->getAttribute("name", _name);
    root->getAttribute("format_version", _formatVersion);

    AL::XmlElement::CList rootBoxs = root->children("BoxInstance", "");

    if(rootBoxs.size() != 1)
    {
      return false;
    }

    _rootBox = BoxInstanceModelPtr(new BoxInstanceModel(rootBoxs.front(), _dir));

    return _rootBox->isValid();
  }

  ChoregrapheProjectModel::ChoregrapheProjectModel(const std::string &dir,
                                         const std::string &name,
                                         const std::string &formatVersion,
                                         BoxInstanceModelPtr rootBox) :
    _p(new ChoregrapheProjectModelPrivate(dir, name, formatVersion, rootBox))
  {
  }

  ChoregrapheProjectModel::~ChoregrapheProjectModel()
  {
    delete _p;
  }

  bool ChoregrapheProjectModel::loadFromFile()
  {
    return _p->loadFromFile();
  }

  const std::string& ChoregrapheProjectModel::getName() const
  {
    return _p->_name;
  }

  const std::string& ChoregrapheProjectModel::getFormatVersion() const
  {
    return _p->_formatVersion;
  }

  BoxInstanceModelPtr ChoregrapheProjectModel::getRootBox() const
  {
    return _p->_rootBox;
  }

  void ChoregrapheProjectModel::setName(const std::string& name)
  {
    _p->_name = name;
  }

  void ChoregrapheProjectModel::setFormatVersion(const std::string& formatVersion)
  {
    _p->_formatVersion = formatVersion;
  }

  void ChoregrapheProjectModel::setRootBox(BoxInstanceModelPtr rootBox)
  {
    _p->_rootBox = rootBox;
  }
}
