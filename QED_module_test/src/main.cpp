#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <tuple>
#include <cmath>
#include <cstdint>
#include <fstream>

#include "commons.h"
#include "species.h"
#include "photons.h"
#include "electrons.h"
#include "positrons.h"

#include "landau_lifshitz.h"
#include "nonlin_breit_wheeler_engine.h"
#include "special_functions.h"
#include "quadrature.h"
#include "rng_wrapper.h"
#include "chi_calc_functions.h"

using namespace std;
using namespace testbed;

const double dt = 0.01;
const int num_steps = 10000;

inline bool is_out(int t_step){return (t_step % 1000 == 0);}

const int64_t seed = 3397169560718639567;

int main(int argc, char** argv){

    cout << "********************Test special functions***************************" << endl;
    cout << "k_1_3 (0.5) = " << picsar::multi_physics::k_1_3 (0.5) << " (exp. 0.989031)" << endl;
    cout << "k_1_3 (1.0) = " << picsar::multi_physics::k_1_3 (1.0) << " (exp. 0.438431)" << endl;
    cout << "k_1_3 (2.0) = " << picsar::multi_physics::k_1_3 (2.0) << " (exp. 0.116545)" << endl;
    cout << "k_2_3 (0.5) = " << picsar::multi_physics::k_2_3 (0.5) << "  (exp. 1.20593 )" << endl;
    cout << "k_2_3 (1.0) = " << picsar::multi_physics::k_2_3 (1.0) << " (exp. 0.494475)" << endl;
    cout << "k_2_3 (2.0) = " << picsar::multi_physics::k_2_3 (2.0) << " (exp. 0.124839)" << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;

    cout << "********************Test numerical integration***********************" << endl;
    cout << "quad(sin(x)**2, 0, 2pi) = " <<
        picsar::multi_physics::quad_a_b([](double x){return sin(x)*sin(x);}, 0, 2*M_PI) << " (exp. 3.14159)" << endl;

    cout << "quad(exp(-x**2), 0, inf.) = " <<
        picsar::multi_physics::quad_a_inf([](double x){return exp(-x*x);},0) << " (exp. 0.886227)" << endl;

    cout << "quad(1/(1+x**2), 0, inf.) = " <<
        picsar::multi_physics::quad_a_inf([](double x){return 1.0/(1+x*x);},0) << " (exp. 1.57079)" << endl;

    cout << "*********************************************************************" << endl;
    cout << endl;

    std::ofstream dump_file{"dump.dat"}; //dump file to store data for later analysis

    cout << "********************QED module testbed***************************" << endl;
    cout << endl;

    //Fix lambda
    double lambda = 0.8 * picsar::multi_physics::_um;

    //Init nonlin_breit_wheeler_engine
    picsar::multi_physics::nonlin_breit_wheeler_engine breit_wheeler_engine{seed, lambda};


    cout << "********************Test RNG ****************************************" << endl;
    picsar::multi_physics::rng_wrapper rngw{seed};
    cout << "3 random numbers in [0, 1):    ";
    cout << rngw.get_unf_0_1() << " " <<  rngw.get_unf_0_1() << " " << rngw.get_unf_0_1() << endl;
    cout << "3 random numbers from exponential distributions:       ";
    cout << rngw.get_exp_l1() << " " <<  rngw.get_exp_l1() << " " << rngw.get_exp_l1() << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;

    cout << "********************Test Schwinger field ****************************" << endl;
    cout << "Calc Schwinger field in code units (exp. 329719 for l=800nm): ";
    cout << picsar::multi_physics::calc_schwinger_given_lambda(800*picsar::multi_physics::_nm) << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;

    cout << "********************Test ChiPhot Functions***************************" << endl;
    cout << "calc Chi for photons (mom=[83.759, 139.311, -230.553], EB=[-166.145, -78.231, -278.856, -279.174, -158.849, -93.826], l = 800 nm, exp. 0.347111844317) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({83.759, 139.311, -230.553},{-166.145, -78.231, -278.856, -279.174, -158.849, -93.826}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "calc Chi for photons (mom=[-2314.45, -2356.30, 546.28], EB=[1230.11, 1638.02, -2911.04, -2203.66, 1243.79, -2830.99], l = 800 nm, exp. 57.2204397969) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({-2314.45, -2356.30, 546.2},{1230.11, 1638.02, -2911.04, -2203.66, 1243.79, -2830.99}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "calc Chi for photons (mom=[9.2627, -25.4575, -10.2246], EB=[2.9271, 10.4293, 3.6103, 1.7439, 1.9778, 17.8799], l = 800 nm, exp. 0.000904147405336) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({9.2627, -25.4575, -10.2246},{2.9271, 10.4293, 3.6103, 1.7439, 1.9778, 17.8799}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;

    cout << "********************Test ChiLept Functions***************************" << endl;
    cout << "calc Chi for leptons (mom=[24.3752, -11.5710, -10.0841], EB=[57.185, -16.6555, 22.4340,6.6911, -23.8724, 13.9934], l = 800 nm, exp. 0.00216716627219670) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({24.3752, -11.5710, -10.0841},{57.185, -16.6555, 22.4340,6.6911, -23.8724, 13.9934}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "calc Chi for leptons (mom=[4.015, 197.287, 141.705], EB=[30.287, 115.740, 120.891, -190.161, -129.115, -57.002], l = 800 nm, exp. 0.166318112874468) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({4.015, 197.287, 141.705},{30.287, 115.740, 120.891, -190.161, -129.115, -57.002}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "calc Chi for leptons (mom=[9.2627, -25.4575, -10.2246], EB=[741.67, -2359.97, 1463.50, 1477.19, -1448.33, 1953.68], l = 800 nm, exp. 16.0114572646993) :" << endl;
    cout << picsar::multi_physics::chi_photon_lambda({-2534.83, 1011.54, -793.04},{741.67, -2359.97, 1463.50, 1477.19, -1448.33, 1953.68}, 0.8 * picsar::multi_physics::_um) << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;


    cout << "********************Test BWFunctions *********************************" << endl;
    // WARNING 2% discrepancy for first case, entirely due to integration for T function!!!!
    auto nn = [](double px,double py, double pz){return sqrt(px*px + py*py + pz*pz);};
    cout << "calc BW dN/dt (mom=[61019.1, -24359.3, 65116.2], EB=[69942.0, 38024.7, -43604.1, -26990.0, 58267.8, -63485.8], l = 800 nm, exp. 7.63488202211) : " << endl;
    cout << breit_wheeler_engine.compute_dN_dt(nn(61019.1, -24359.3, 65116.2),
        picsar::multi_physics::chi_photon_lambda({61019.1, -24359.3, 65116.2},{69942.0, 38024.7, -43604.1, -26990.0, 58267.8, -63485.8}, 0.8 * picsar::multi_physics::_um)) << endl;
    cout << "calc BW dN/dt (mom=[-965.61, -3975.11, 6917.22], EB=[11.17, -2117.72, -1407.19, 6259.79, 7557.54, 773.11], l = 800 nm, exp. 3.51855878777) : " << endl;
    cout << breit_wheeler_engine.compute_dN_dt(nn(-965.61, -3975.11, 6917.22),
        picsar::multi_physics::chi_photon_lambda({-965.61, -3975.11, 6917.22},{11.17, -2117.72, -1407.19, 6259.79, 7557.54, 773.11}, 0.8 * picsar::multi_physics::_um)) << endl;
    cout << "calc BW dN/dt (mom=[149.825, 933.115, -538.195], EB=[931.686, -861.074, 944.652, 531.406, 670.933, 660.057], l = 800 nm, exp. 1.50648551484) : " << endl;
    cout << breit_wheeler_engine.compute_dN_dt(nn(149.825, 933.115, -538.195),
        picsar::multi_physics::chi_photon_lambda({149.825, 933.115, -538.195},{931.686, -861.074, 944.652, 531.406, 670.933, 660.057}, 0.8 * picsar::multi_physics::_um)) << endl;
    cout << "calc BW dN/dt (mom=[-44.4546, -0.2033, 94.5843], EB=[39.8996, -29.2501, 58.7720, 44.3417, 15.5024, 29.4024], l = 800 nm, exp. 4.69766211952e-73) : " << endl;
    cout << breit_wheeler_engine.compute_dN_dt(nn(-44.4546, -0.2033, 94.5843),
        picsar::multi_physics::chi_photon_lambda({-44.4546, -0.2033, 94.5843},{39.8996, -29.2501, 58.7720, 44.3417, 15.5024, 29.4024}, 0.8 * picsar::multi_physics::_um)) << endl;
    cout << "calc BW dN/dt (mom=[6.81696,9.68933,2.81229], EB=[-4.89986,-9.65535,3.69471, 8.89549,-5.46574,-6.75393], l = 800 nm, exp. 0.0) : " << endl;
    cout << breit_wheeler_engine.compute_dN_dt(nn(6.81696,9.68933,2.81229),
        picsar::multi_physics::chi_photon_lambda({6.81696,9.68933,2.81229},{-4.89986,-9.65535,3.69471, 8.89549,-5.46574,-6.75393}, 0.8 * picsar::multi_physics::_um)) << endl;
    cout << "*********************************************************************" << endl;
    cout << endl;

    vector<shared_ptr<species>> specs;
    //Init a photon
    auto ptr_phot1 = make_shared<photons>("phot1");
    ptr_phot1->add_particle({0,0,0},{0.0,0.0,0.0});
    specs.emplace_back(ptr_phot1);

    //Create LL pusher using multi_physics library
    auto pusher =
    [lambda](momenta_list& mom, const em_field_list& fields,  double mass, double charge, double dt)->void{
        picsar::multi_physics::boris_plus_landau_lifshitz_push(
        mom[0], mom[1], mom[2],
        fields[0], fields[1], fields[2],
        fields[3], fields[4], fields[5],
        mass, charge, dt, lambda);
    };

    //Init an electron
    auto ptr_ele1 = make_shared<electrons>("ele1");
    ptr_ele1->add_particle({0,0,0},{0,100,0.0});
    //Replace pusher
    ptr_ele1->replace_pusher_momenta(pusher);
    specs.emplace_back(ptr_ele1);

    //Init a positron
    auto ptr_pos1 = make_shared<positrons>("pos1");
    ptr_pos1->add_particle({0,0,0},{0.0,0.0,0.0});
    //ptr_pos1->replace_pusher_momenta(pusher);
    specs.emplace_back(ptr_pos1);

    // Main loop
    for (int i = 0; i < num_steps; i++){

    //Dump_file output of a particle position for debug purposes
        auto tpos = ptr_ele1->get_copy_of_positions();
        dump_file << tpos[0][0] << " " << tpos[1][0] << " " << tpos[2][0] << " ";
        auto tmom = ptr_ele1->get_copy_of_momenta();
        dump_file << tmom[0][0] << " " << tmom[1][0] << " " << tmom[2][0] << endl;

        for (auto& sp : specs)
            sp->push_momenta(dt);

        for (auto& sp : specs)
            sp->calc_fields([](position pos, double ttime){return em_field{0,0,0,100.0,0.0,0.0};}, i*dt);

        for (auto& sp : specs)
            sp->push_positions(dt);

        if(is_out(i)){
            for (auto& sp : specs)
                sp->print_on_disk("out", i);
        }
    }

    cout << "*****************************************************************" << endl;

    dump_file.close();

    return 0;
}

//Nice field!
//sp->calc_fields([](position pos, double ttime){return em_field{0,cos(pos[0]),sin(pos[0]),0,sin(pos[0]),-cos(pos[0])};}, i*dt);
