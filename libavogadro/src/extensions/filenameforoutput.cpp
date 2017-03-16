#include <avogadro/glwidget.h>
#include <avogadro/molecule.h>

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QFile>

/// \brief Ask the user where he wants to save data.  
///    \param fileType restrict the files that can be seen in the dialog window to this type e.g. "text"
///    \param extension show files with this extension; also, save the file with this extension
///    \param baseName default basename
///    \param windowTitle title of the window that gets passed to Qt tr()
///    \return the complete path where the user wants to save data.  
QString outputFilename(Avogadro::Molecule * molecule, QWidget * widget, QString fileType, QString extension, QString baseName, const char * windowTitle){
    QFileInfo defaultFile(molecule->fileName());
    QString defaultPath = defaultFile.canonicalPath();
    if (defaultPath.isEmpty()){
        defaultPath = QDir::homePath();
    }

    QString defaultFileName = defaultPath + '/' + baseName + "." + extension;
    QString fileName = QFileDialog::getSaveFileName(widget, QObject::tr(windowTitle),
        defaultFileName, fileType + " (*." + extension + ")");

    return fileName;
}


