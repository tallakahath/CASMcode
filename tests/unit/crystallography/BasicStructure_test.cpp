#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

/// What is being tested:
#include "casm/crystallography/BasicStructure.hh"

/// What is being used to test it:
#include "ZrOProj.hh"
#include "casm/crystallography/Site.hh"
#include "casm/app/AppIO.hh"
#include "casm/casm_io/VaspIO.hh"
#include "casm/crystallography/SupercellEnumerator.hh"

using namespace CASM;

/** PRIM1 *****************************
Face-centered Cubic (FCC, cF)
1.0
0 2.0 2.0
2.0 0 2.0
2.0 2.0 0
1
D
0.00 0.00 0.00 A B C
***************************************/

void prim1_read_test(BasicStructure<Site> &struc) {

  double tol = 1e-5;

  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[0], Eigen::Vector3d(0.0, 2.0, 2.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[1], Eigen::Vector3d(2.0, 0.0, 2.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[2], Eigen::Vector3d(2.0, 2.0, 0.0), tol), true);
  BOOST_CHECK_EQUAL(struc.basis.size(), 1);

  // basis site 0 has three possible occupants
  BOOST_CHECK_EQUAL(struc.basis[0].site_occupant().size(), 3);

  std::string check_name[3] = {"A", "B", "C"};

  for(int i = 0; i < 3; i++) {
    // occupants are Molecule with name "A", etc.
    // Molecule are composed of AtomPosition
    // An AtomPosition 'is' a Coordinate with a Specie
    BOOST_CHECK_EQUAL(struc.basis[0].site_occupant()[i].name, check_name[i]);
    BOOST_CHECK_EQUAL(almost_equal(struc.basis[0].site_occupant()[i][0].const_frac(), Eigen::Vector3d(0.0, 0.0, 0.0), tol), true);
    BOOST_CHECK_EQUAL(struc.basis[0].site_occupant()[i][0].specie.name, check_name[i]);
  }

  // FCC motif
  MasterSymGroup factor_grp;
  struc.generate_factor_group(factor_grp, tol);
  BOOST_CHECK_EQUAL(48, factor_grp.size());
}

/** PRIM2 *****************************
Face-centered Cubic (FCC, cF)
1.0
4.0 0.0 0.0
0.0 4.0 0.0
0.0 0.0 4.0
2 1 1
D
0.00 0.00 0.00 A B C :: A
0.5 0.5 0.0 A B C :: A
0.5 0.0 0.5 A B C :: B
0.0 0.5 0.5 A B C :: C
***************************************/

void prim2_read_test(BasicStructure<Site> &struc) {

  double tol = 1e-5;

  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[0], Eigen::Vector3d(4.0, 0.0, 0.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[1], Eigen::Vector3d(0.0, 4.0, 0.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[2], Eigen::Vector3d(0.0, 0.0, 4.0), tol), true);
  BOOST_CHECK_EQUAL(struc.basis.size(), 4);

  // basis site 0 has three possible occupants
  BOOST_CHECK_EQUAL(struc.basis[0].site_occupant().size(), 3);

  std::string check_name[3] = {"A", "B", "C"};
  int check_value[4] = {0, 0, 1, 2};

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 3; j++) {
      // occupants are Molecule with name "A", etc.
      // Molecule are composed of AtomPosition
      // An AtomPosition 'is' a Coordinate with a Specie
      BOOST_CHECK_EQUAL(struc.basis[i].site_occupant()[j].name, check_name[j]);
      BOOST_CHECK_EQUAL(almost_equal(struc.basis[i].site_occupant()[j][0].const_frac(), Eigen::Vector3d(0.0, 0.0, 0.0), tol), true);
      BOOST_CHECK_EQUAL(struc.basis[i].site_occupant()[j][0].specie.name, check_name[j]);

    }
    BOOST_CHECK_EQUAL(struc.basis[i].site_occupant().value(), check_value[i]);
  }

  // ordering on FCC motif
  MasterSymGroup factor_grp;
  struc.generate_factor_group(factor_grp, tol);
  BOOST_CHECK_EQUAL(16, factor_grp.size());
}


/** POS1 *****************************
Face-centered Cubic (FCC, cF)
1.0
4.0 0.0 0.0
0.0 4.0 0.0
0.0 0.0 4.0
A B C
2 1 1
D
0.00 0.00 0.00 A
0.5 0.5 0.0 A
0.5 0.0 0.5 B
0.0 0.5 0.5 C
***************************************/


/** POS1_vasp5_out ********************
Face-centered Cubic (FCC, cF)
 1.00000000
       4.00000000      0.00000000      0.00000000
       0.00000000      4.00000000      0.00000000
       0.00000000      0.00000000      4.00000000
A B C
2 1 1
Direct
   0.0000000   0.0000000   0.0000000
   0.5000000   0.5000000   0.0000000
   0.5000000   0.0000000   0.5000000
   0.0000000   0.5000000   0.5000000
***************************************/


void pos1_read_test(BasicStructure<Site> &struc) {

  double tol = 1e-5;

  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[0], Eigen::Vector3d(4.0, 0.0, 0.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[1], Eigen::Vector3d(0.0, 4.0, 0.0), tol), true);
  BOOST_CHECK_EQUAL(almost_equal(struc.lattice()[2], Eigen::Vector3d(0.0, 0.0, 4.0), tol), true);
  BOOST_CHECK_EQUAL(struc.basis.size(), 4);

  std::string check_name[4] = {"A", "A", "B", "C"};

  for(int i = 0; i < 4; i++) {
    // basis site 0 and 1 have one possible occupant
    BOOST_CHECK_EQUAL(struc.basis[i].site_occupant().size(), 1);

    // occupants are Molecule with name "A", etc.
    // Molecule are composed of AtomPosition
    // An AtomPosition 'is' a Coordinate with a Specie
    BOOST_CHECK_EQUAL(struc.basis[i].site_occupant()[0].name, check_name[i]);
    BOOST_CHECK_EQUAL(almost_equal(struc.basis[i].site_occupant()[0][0].const_frac(), Eigen::Vector3d(0.0, 0.0, 0.0), tol), true);
    BOOST_CHECK_EQUAL(struc.basis[i].site_occupant()[0][0].specie.name, check_name[i]);
  }

  // FCC structure
  MasterSymGroup factor_grp;
  struc.generate_factor_group(factor_grp, tol);
  BOOST_CHECK_EQUAL(16, factor_grp.size());
}

BOOST_AUTO_TEST_SUITE(BasicStructureSiteTest)

BOOST_AUTO_TEST_CASE(PRIM1Test) {

  fs::path testdir = "tests/unit/crystallography";

  // Read in test PRIM and run tests
  BasicStructure<Site> struc(fs::path(testdir / "PRIM1"));
  prim1_read_test(struc);

  // Write test PRIM back out
  fs::path tmp_file = testdir / "PRIM1_out";
  write_prim(struc, tmp_file, FRAC);

  // Read new file and run tests again
  BasicStructure<Site> struc2(read_prim(tmp_file));
  prim1_read_test(struc2);

}

BOOST_AUTO_TEST_CASE(PRIM2Test) {

  fs::path testdir = "tests/unit/crystallography";

  // Read in test PRIM and run tests
  BasicStructure<Site> struc(fs::path(testdir / "PRIM2"));
  prim2_read_test(struc);
}

BOOST_AUTO_TEST_CASE(PRIM3Test) {

  fs::path testdir = "tests/unit/crystallography";

  // Read in an incorrectly formatted PRIM and check that an exception is thrown
  BOOST_CHECK_THROW(BasicStructure<Site>(fs::path(testdir / "PRIM3")), std::runtime_error);

}

BOOST_AUTO_TEST_CASE(POS1Test) {

  fs::path testdir = "tests/unit/crystallography";

  // Read in test PRIM and run tests
  BasicStructure<Site> struc(fs::path(testdir / "POS1"));
  pos1_read_test(struc);

  // Write test PRIM back out
  fs::path tmp_file = testdir / "POS1_out";
  fs::ofstream sout(tmp_file);
  VaspIO::PrintPOSCAR printer(struc);
  printer.set_append_atom_names_off();
  printer.print(sout);
  sout.close();

  // Read new file and run tests again
  BasicStructure<Site> struc2(fs::path(testdir / "POS1_out"));
  pos1_read_test(struc2);

}

BOOST_AUTO_TEST_CASE(POS1Vasp5Test) {

  fs::path testdir = "tests/unit/crystallography";

  // Read in test PRIM and run tests
  BasicStructure<Site> struc(fs::path(testdir / "POS1"));
  pos1_read_test(struc);

  // Write test PRIM back out
  fs::path tmp_file = testdir / "POS1_vasp5_out";
  fs::ofstream sout(tmp_file);
  VaspIO::PrintPOSCAR(struc).print(sout);
  sout.close();

  // Read new file and run tests again
  BasicStructure<Site> struc2(fs::path(testdir / "POS1_vasp5_out"));
  pos1_read_test(struc2);

}

BOOST_AUTO_TEST_CASE(IsPrimitiveTest) {

  Structure prim(test::ZrO_prim());

  const SymGroup effective_pg = prim.factor_group();

  ScelEnumProps enum_props(1, 6);
  SupercellEnumerator<Lattice> scel_enum(prim.lattice(), effective_pg, enum_props);

  for(auto it = scel_enum.begin(); it != scel_enum.end(); ++it) {

    Structure super = prim.create_superstruc(*it);
    BOOST_CHECK_EQUAL(super.lattice().is_right_handed(), true);

    Structure new_prim;
    super.is_primitive(new_prim);
    BOOST_CHECK_EQUAL(new_prim.lattice().is_right_handed(), true);
    BOOST_CHECK_EQUAL(new_prim.lattice().is_right_handed(), super.lattice().is_right_handed());
  }

}

BOOST_AUTO_TEST_SUITE_END()
