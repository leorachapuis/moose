//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestPostprocessor.h"
#include "MooseTestAppTypes.h"
#include "Conversion.h"

registerMooseObject("MooseTestApp", TestPostprocessor);

template <>
InputParameters
validParams<TestPostprocessor>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  MooseEnum test_type("grow use_older_value report_old custom_execute_on");
  params.addRequiredParam<MooseEnum>("test_type", test_type, "The type of test to perform");
  params.addParam<PostprocessorName>("report_name",
                                     "The name of the postprocessor value to report");
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_JUST_GO);
  params.setDocString("execute_on", exec.getDocString());
  return params;
}

TestPostprocessor::TestPostprocessor(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _test_type(getParam<MooseEnum>("test_type")),
    _old_val(getPostprocessorValueOldByName(name())),
    _older_val(getPostprocessorValueOlderByName(name()))
{
  if (_test_type == "report_old" && !isParamValid("report_name"))
    mooseError("Must set 'report_name' parameter when using the 'report_old' test type.");
}

Real
TestPostprocessor::getValue()
{
  if (_test_type == "grow")
  {
    if (_t_step == 0)
      return 1;
    return _old_val + 1;
  }

  else if (_test_type == "use_older_value")
  {
    if (_t_step == 0)
      return 1;
    return _old_val + _older_val;
  }

  else if (_test_type == "report_old")
    return getPostprocessorValueOld("report_name");

  else if (_test_type == "custom_execute_on")
  {
    _console << "Flag Name: " << EXEC_JUST_GO << std::endl;
    return ++_execute_count;
  }
  // This should not be attainable
  else
  {
    mooseError("Invalid test type.");
    return 0.0;
  }
}
