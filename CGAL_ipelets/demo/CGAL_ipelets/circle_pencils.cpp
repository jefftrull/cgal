// Copyright (c) 2005-2009  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Olivier Devillers
 
#include <CGAL/Cartesian.h>
#include <CGAL/CGAL_Ipelet_base.h> 
#include <CGAL/Object.h>

#include "include/CGAL_ipelets/pencils.h"


namespace CGAL_pencils{

typedef CGAL::Cartesian<double>                                               Kernel;
// --------------------------------------------------------------------

const std::string sublabel[] = {
  "Circle in pencil through orthogonal to circle", "Circle  orthogonal to three circles", "Help"
};

const std::string helpmsg[] = {
  "Draw the circle orthogonal to a circle (primary selection) in the pencil generated by two circles",
  "Draw the circle orthogonal to three circles"
};

class pencilIpelet 
  : public CGAL::Ipelet_base<Kernel,3> {
public:
  pencilIpelet() 
    :CGAL::Ipelet_base<Kernel,3>("Pencils of circles",sublabel,helpmsg){}
  void protected_run(int);
};
// --------------------------------------------------------------------

void pencilIpelet::protected_run(int fn)
{
  Circle_2 circ;     //constructed circle:
  
  if (fn==2) {
    show_help();
    return;
  } 
  
  Circle_2  c,c0,c1,c2;
  std::list<Point_2> pt_list,pt_list1;
  std::list<Circle_2> cir_list,cir_list1;


  int i=get_IpePage()->primarySelection(); 
  read_one_active_object(get_IpePage()->object(i),CGAL::dispatch_or_drop_output<Point_2,Circle_2>(
       std::back_inserter(pt_list1),
       std::back_inserter(cir_list1))); 
  
  std::list<Point_2>::iterator it1=pt_list1.begin();
  std::list<Circle_2>::iterator cit1=cir_list1.begin();

  if (pt_list1.empty() && cir_list1.empty()){
  print_error_message(("Primary selection must be a mark or a circle"));
    return;
  }
  if (it1!=pt_list1.end())  {c=Circle_2(*it1,0);} else c=*cit1;

  Iso_rectangle_2 bbox=
  read_active_objects(
		      CGAL::dispatch_or_drop_output<Point_2,Circle_2>(
      std::back_inserter(pt_list),
      std::back_inserter(cir_list)
    )
  );

  std::list<Point_2>::iterator it=pt_list.begin();
  std::list<Circle_2>::iterator cit=cir_list.begin();


  // read c0
  if (it!=pt_list.end())  { c0=Circle_2(*it,0); ++it;}
  else if (cit!=cir_list.end())  { c0=*cit; ++cit;}
  else {print_error_message(("Not enough marks or circles selected")); return;}
  // read c1
  if (it!=pt_list.end())  { c1=Circle_2(*it,0); ++it;}
  else if (cit!=cir_list.end())  { c1=*cit; ++cit;}
  else {print_error_message(("Not enough marks or circles selected")); return;}
  // read c2
  if (it!=pt_list.end())  { c2=Circle_2(*it,0); ++it;}
  else if (cit!=cir_list.end())  { c2=*cit; ++cit;}
  else {print_error_message(("Not enough marks or circles selected")); return;}
  if ((it!=pt_list.end())||(cit!=cir_list.end()))
    {print_error_message(("Warning: more than three marks or circles selected"));}
  
  // c is the primary selection
  if (c==c1) c1=c0;
  if (c==c2) c2=c0;


  switch(fn){
  case 0:
    // Circle orthogonal to circle c in pencil generated by c1 c2
    circ = compute_circle_in_pencil<Kernel>(c,c1,c2);
    break;
  case 1:
    // Circle orthogonal to three circles
    circ = compute_circle_orthogonal<Kernel>(c,c1,c2);
    break;    
    
  }     //end of switch

  std::cout<<circ<<std::endl;

  if (circ.squared_radius()>0){
    draw_in_ipe(circ);
  }else{
    print_error_message(("computed circle is imaginary"));
  }
}
}

CGAL_IPELET(CGAL_pencils::pencilIpelet)
