// Oven Sensor

use <CircuitBoard.scad>
use <WemosD1mini.scad>
use <FlameSensor.scad>
use <Dht11Sensor.scad>

pcb_20x14 = [60, 40, 1];

module oven_sensor(def=pcb_20x14,n=[20,14], d=2.54,holes=false) {
  if (holes==false)
    circuit_board(def);
  if (holes==true) {
    difference() {
      circuit_board(def);
      oven_sensor_holes(def=def,n=n);
    }
  }
  //
  // DHT11 connector (3 pins)
  oven_sensor_connector(def, n, 1, 3, rc=0);
  // FLAME connector (4 pins)
  oven_sensor_connector(def, n, 10, 13, rc=1);
  // WEMOS D1 mini connectors (2 x 8 pins)
  oven_sensor_connector(def, n, 11, 18, rc=14, hor=false);
  oven_sensor_connector(def, n, 2, 9, rc=14, hor=false);
  //
  // WEMOS D1 mini PCB
  translate(holexy(def,n,d,rc=7,f=0)+[0,d/2,8+d])
  wemos_D1_mini(con=true);
  // DHT11 sensor PCB
  w=0.5;
  translate(holexy(def,n,d,rc=2,f=0)+[-d/2-w/2,-d-d/2+w/2,2.5*d+1.28])
  rotate([0,-90,0])
  dht11_sensor();
  // FLAME sensor PCB
  translate(holexy(def,n,d,rc=3,f=7)+[-d/2,0,2.5*d+1.28+0.25])
  rotate([0,-90,0])
  flame_sensor();
}

oven_sensor(holes=true);

module oven_sensor_mount(def) {
  circuit_board_mount(def);
}

function hole00(def, n, d) = 
[ (def[0]-(n[0]-1)*d)/2, 
  (def[1]-(n[1]-1)*d)/2, 
  0];

module oven_sensor_holes(def,d=2.54,w=0.5, n) {
  translate(hole00(def,n,d))
  union()
     for (x0 = [0: d : (n[0]-1)*d]) {
       for (xl = [0: d : (n[1]-1)*d]) {
          translate([x0, xl, -w])
            cylinder(h=4*w, d=2*w, $fn=24);
      }
   }
}

module male_connectors(d=2.54,w=0.5,n=6,h=8){
  difference() {
    color("DarkGray") 
    translate([-d,0,h/2])
    cube([d, n*d, h], center=true);
    for (xl = [0: d : (n-1)*d]) {
      translate([-d-w, xl-(n-1)*d/2-w, 1-d])
      cube([2*w, 2*w, 4*h]);
     }
  }
}

// rc and f are swapped?
function holexy(def,n,d,f,rc) =
    hole00(def,n,d)+ [(rc+1)*d, (f+1)*d, def[2]];

module oven_sensor_connector(def,n,f,t,rc,hor=true) {
  d=2.54;
  w=0.5;
  translate(holexy(def,n,d,f,rc))
  if (hor == false)
      translate([-d/2,0,0]) rotate([0, 0, 90])male_connectors(n=t-f+1); 
  else
    male_connectors(n=t-f+1); 
}

