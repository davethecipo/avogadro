/**********************************************************************
  Selection Consecutive Distance Plugin for Avogadro

  Copyright (C) 2017 by Davide Olianas

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.cc/>

  Some code is based on Open Babel
  For more information, see <http://openbabel.sourceforge.net/>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 ***********************************************************************/

#include "selectionconsecutivedistance.h"
#include <avogadro/primitive.h>
#include <avogadro/primitivelist.h>
#include <avogadro/color.h>
#include <avogadro/glwidget.h>
#include <avogadro/molecule.h>
#include <avogadro/atom.h>

#include <Eigen/Core>

#include <openbabel/mol.h>

#include <QAction>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QList>

#include "filenameforoutput.cpp"

using namespace std;
using namespace OpenBabel;

namespace Avogadro {

  SelectionConsecutiveDistanceExtension::SelectionConsecutiveDistanceExtension(QObject *parent) : Extension(parent)
  {
    QAction *action = new QAction(this);
    action->setText(tr("Export distance between consecutive selected atoms"));
    m_actions.append(action);

    action = new QAction( this );
    action->setSeparator(true);
    m_actions.append( action );
  }

  SelectionConsecutiveDistanceExtension::~SelectionConsecutiveDistanceExtension()
  {
  }

  QList<QAction *> SelectionConsecutiveDistanceExtension::actions() const
  {
    return m_actions;
  }

  QString SelectionConsecutiveDistanceExtension::menuPath(QAction *) const
  {
    return tr("E&xtensions");
  }

  void SelectionConsecutiveDistanceExtension::setMolecule(Molecule *molecule)
  {
    m_molecule = molecule;
  }

  QUndoCommand* SelectionConsecutiveDistanceExtension::performAction(QAction *action, GLWidget *widget)
  {


    QString fileName = outputFilename(m_molecule, widget, QString("text") , QString("txt") , QString("bonds") , "Save distance between atoms to" );

    QFile file(fileName);
    file.open( QIODevice::WriteOnly | QIODevice::Text );
    QTextStream outStream(&file);
    outStream << "Bond number," << '\t' << "distance [Angstrom]," << '\t' << "From atom," << '\t' << "To atom"  << endl;

    PrimitiveList selectedPrimitives = widget->selectedPrimitives();
    const QList<Primitive *> AtomsList = selectedPrimitives.subList(Primitive::AtomType);
    for(int i=0; i<AtomsList.count()-1; i++) {
      // p is a pointer to a Primitive; save the memory location of the i-th element in it
      const Primitive *p = AtomsList.at(i);  
      const Atom *selected = static_cast<const Atom *>(p);

      // point to the next element
      p = AtomsList.at(i+1);
      const Atom *nextAtom = static_cast<const Atom *>(p);

      const Eigen::Vector3d *start = selected->pos();
      const Eigen::Vector3d *end = nextAtom->pos();
      Eigen::Vector3d diff;
      diff = *end - *start;
      double length = diff.norm(); 
      outStream << i+1 << '\t' << length << '\t' << selected->index()+1 << '\t' << nextAtom->index()+1 << endl;
    }

    file.close(); // flush buffer!

    // Selections are per-view and as such are not saved or undo-able
    return NULL;
  }

} // end namespace Avogadro

Q_EXPORT_PLUGIN2(SelectionConsecutiveDistanceExtension, Avogadro::SelectionConsecutiveDistanceExtensionFactory)
