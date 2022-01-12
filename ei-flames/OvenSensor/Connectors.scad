// some connectors

module female_connectors_90(d=2.54,w=0.5,n=4){
    x=3*d;
    y=2.5*d;
    union() {
        color("DarkGray") 
        translate([-d,0,d/2])
        cube([d, n*d, d], center=true);
        color("White") 
        for (xl = [0: d : (n-1)*d]) {
            translate([-d-w/2, xl-(n-1)*d/2, 1-d]) {
            cube([w, w, y]);
            translate([-x+w,0,y]) cube([x, w, w]);}
        }
    }
}

