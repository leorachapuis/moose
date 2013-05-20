#ifndef CONTACTMASTER_H
#define CONTACTMASTER_H

// Moose Includes
#include "DiracKernel.h"
#include "PenetrationLocator.h"

//Forward Declarations
class ContactMaster;

template<>
InputParameters validParams<ContactMaster>();

enum ContactModel
{
  CM_INVALID,
  CM_FRICTIONLESS,
  CM_GLUED,
  CM_COULOMB,
  CM_TIED,
  CM_EXPERIMENTAL
};

enum ContactFormulation
{
  CF_INVALID,
  CF_DEFAULT,
  CF_PENALTY,
  CF_AUGMENTED_LAGRANGE
};

class ContactMaster : public DiracKernel
{
public:
  ContactMaster(const std::string & name, InputParameters parameters);

  virtual void jacobianSetup();
  virtual void timestepSetup();

  virtual void addPoints();
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  virtual void updateContactSet(bool beginning_of_step = false);

protected:
  const unsigned int _component;
  const ContactModel _model;
  const ContactFormulation _formulation;
  PenetrationLocator & _penetration_locator;

  const Real _penalty;
  const Real _friction_coefficient;
  const Real _tension_release;
  bool _updateContactSet;
  Real _time_last_called;

  NumericVector<Number> & _residual_copy;

  std::map<Point, PenetrationLocator::PenetrationInfo *> _point_to_info;

  unsigned int _x_var;
  unsigned int _y_var;
  unsigned int _z_var;

  RealVectorValue _vars;
};

ContactModel contactModel(const std::string & the_name);
ContactFormulation contactFormulation(const std::string & the_name);

#endif //CONTACTMASTER_H
