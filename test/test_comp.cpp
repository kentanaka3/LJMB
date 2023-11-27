#include "gtest/gtest.h"
#include "comp.h"
#include "structs.h"

#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>



class ComputationsTest:
  public::testing::Test {
    protected:
    mdsys_t * sys;
    void SetUp() {
      sys = new mdsys_t;
      sys->natoms = 2;
      sys->mass = 1.0;
      sys->ekin = 0.0;
      sys->temp = 0.0;
      sys->box = 17.1580;
      sys->rcut = 8.5;
      sys->epsilon = 0.2379;
      sys->sigma = 3.405;
      sys->epot = -160.48438434;

      sys->dt = MVSQ2E;
      sys->rx = new double[sys->natoms];
      sys->vx = new double[sys->natoms];
      sys->fx = new double[sys->natoms];
      sys->ry = new double[sys->natoms];
      sys->vy = new double[sys->natoms];
      sys->fy = new double[sys->natoms];
      sys->rz = new double[sys->natoms];
      sys->vz = new double[sys->natoms];
      sys->fz = new double[sys->natoms];
      sys->rx[0] = -1.0;
      sys->rx[1] = 1.0;
      sys->ry[0] = -1.0;
      sys->ry[1] = 1.0;
      sys->rz[0] = -1.0;
      sys->rz[1] = 1.0;
      sys->vx[0] = 1.0;
      sys->vx[1] = 1.0;
      sys->vy[0] = 0.0;
      sys->vy[1] = 0.0;
      sys->vz[0] = 0.0;
      sys->vz[1] = 0.0;
      sys->fx[0] = 1.0;
      sys->fx[1] = 0.2;
      sys->fy[0] = 1.0;
      sys->fy[1] = 0.2;
      sys->fz[0] = 1.0;
      sys->fz[1] = 0.2;
    }
    void TearDown() {
      delete[] sys->rx;
      delete[] sys->vx;
      delete[] sys->fx;
      delete[] sys->ry;
      delete[] sys->vy;
      delete[] sys->fy;
      delete[] sys->rz;
      delete[] sys->vz;
      delete[] sys->fz;

      delete sys;
    }
  };
  TEST_F(ComputationsTest, KineticE) {
    ASSERT_NE(sys, nullptr);
    ASSERT_DOUBLE_EQ(sys->vx[0], 1.0);
    ASSERT_DOUBLE_EQ(sys->vx[1], 1.0);
    ekin(sys);
    ASSERT_DOUBLE_EQ(sys->ekin, MVSQ2E);
    ASSERT_DOUBLE_EQ(sys->temp, 2.0*MVSQ2E/(3.0*KBOLTZ));
    printf("gone here");
  }

  TEST_F(ComputationsTest, Force) {
    ASSERT_NE(sys,nullptr);
    ASSERT_DOUBLE_EQ(sys->rx[0], -1.0);
    ASSERT_DOUBLE_EQ(sys->rx[1], 1.0);
    ASSERT_DOUBLE_EQ(sys->rx[0], -1.0);
    ASSERT_DOUBLE_EQ(sys->rx[1], 1.0);
    ASSERT_DOUBLE_EQ(sys->rx[0], -1.0);
    ASSERT_DOUBLE_EQ(sys->rx[1], 1.0);
    printf("gone there x2");
    ASSERT_DOUBLE_EQ(sys->epot, -160.48438434);
    force(sys);
    ASSERT_DOUBLE_EQ(sys->epot, -0.084193223448705001);
    ASSERT_DOUBLE_EQ(sys->fx[0], -0.68986272432173701);
    ASSERT_DOUBLE_EQ(sys->fx[1], 0.68986272432173701);
    ASSERT_DOUBLE_EQ(sys->fy[0], -0.68986272432173701);
    ASSERT_DOUBLE_EQ(sys->fy[1], 0.68986272432173701);
    ASSERT_DOUBLE_EQ(sys->fz[0], -0.68986272432173701);
    ASSERT_DOUBLE_EQ(sys->fz[1], 0.68986272432173701);
  }



