// WEMOS D1 mini R2 board (ESP8266F)
pcb_D1 = [34,26, 1];

module female_connectors(d=2.54,w=0.5,n=6){
    union() {
        color("DarkGray") 
        translate([-d,0,d/2])
        cube([d, n*d, d], center=true);
        for (xl = [0: d : (n-1)*d]) {
            translate([-d-w/2, xl-(n-1)*d/2, 1-d])
            color("White") cube([w, w, 4*d]);
        }
    }
}

module wemos_D1_mini_holes(def=pcb_D1,d=2.54,w=0.5,n=8){
    left=[2*d, def[1]/2+d,0];
    rotate([0,180,0])   // upside done
    rotate([0,0,90]) {   // align to pcb_D1[0]
    translate(left) // left
    union() {
        translate([-d/2,d/2,0])
        for (xl = [0: d : (n-1)*d]) {
            translate([-d-w/2, xl-(n-1)*d/2, -d])
            cylinder(h=2*d, r=w, $fn=24);
        }
        translate([def[1]-3*d/2,d/2,0])
        for (xl = [0: d : (n-1)*d]) {
            translate([-d-w/2, xl-(n-1)*d/2, -d])
            cylinder(h=2*d, r=w, $fn=24);
        }
    }
}
}

module wemos_D1_mini(def=pcb_D1,d=2.54,w=0.5,con=false) {
    usb_b = [5,6,3]; // 12,10,5
    color("SteelBlue") difference() {
        cube(pcb_D1);
        wemos_D1_mini_holes();
    }
    if (con == true) {
      left=[2*d, def[1]/2+d,0];
      rotate([0,180,0])   // upside done
      rotate([0,0,90]) {   // align to pcb_D1[0]
      translate(left+[-(d+w)/2, 2*w,0]) // left
      female_connectors(n=8);
      translate(left+[def[0]-5*d+w, 2*w,0]) // right
      female_connectors(n=8);
    }
    }
    // USB connector
    color("Gray") translate([def[0]-usb_b[0], def[1]/2-usb_b[1]/2,0]) cube(usb_b);
    // on board antenna
    color("White") translate([0,def[1]-10-1-d,def[2]]) cube([d,10,d/2]); 
    
}
wemos_D1_mini(con=true);


