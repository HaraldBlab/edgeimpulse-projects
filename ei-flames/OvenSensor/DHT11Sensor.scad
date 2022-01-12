// the KY-015 DHT11 sensor
pcb_KY015 = [27, 12, 1];

use <Connectors.scad>

module dht11_sensor_connectors(def=pcb_KY015,d=2.54,w=0.5) {
    // the connector
    left=[d+d/2+w/2,def[1]/2,0];
    translate(left+[-w/2,-w/2,-w+1.5])
    female_connectors_90(n=3);
}

module dht11_sensor_holes(def=pcb_KY015,d=2.54,w=0.5,n=3) {
    left=[d+d/2+w/2,def[1]/2,0];
    translate(left)
    for (xl = [0: d : (n-1)*d]) {
        translate([-d-w/2, xl-(n-1)*d/2, -d])
        cylinder(h=2*d, r=w, $fn=24);
    }
}

module dht11_sensor_cube(cube=[16, 12, 8]) {
    cube(cube);
}

module dht11_sensor(def=pcb_KY015) {
    color("DarkGray")
    // the board
    difference() {
    cube(def);
    translate([7, def[1]/2, -1.5]) 
        cylinder(d=3,h=3, $fn=24);
    dht11_sensor_holes(def=def);
    }
    dht11_sensor_connectors(def=def);
    // the sensor
    sensor=[15, 12, 8];
    color("SteelBlue")
    translate([def[0]-sensor[0]-2, (def[1]-sensor[1])/2, def[2]])
    dht11_sensor_cube(cube=sensor);
}

dht11_sensor();
        
