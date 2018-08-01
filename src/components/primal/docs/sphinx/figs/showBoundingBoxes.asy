// To turn this Asymptote source file into an image for inclusion in
// Axom's documentation,
// 1. run Asymptote:
//    asy -f png showBoundingBoxes.asy
// 2. Optionally, use ImageMagick to convert the white background to transparent:
//    convert showBoundingBoxes.asy -transparent white showBoundingBoxes.asy

// preamble
settings.render = 6;
import three;
size(6cm, 0);

// projection
currentprojection = perspective((4, -1.8, 3), (0.07, 0.07, 1));

// axes
draw(O -- 4X, arrow=Arrow3(DefaultHead2), L=Label("$x$", position=EndPoint));
draw(O -- 7Y, arrow=Arrow3(), L=Label("$y$", position=EndPoint));
draw(O -- 5Z, arrow=Arrow3(), L=Label("$z$", position=EndPoint));

// points
triple[] points = new triple[6];
points[0] = (0.6,1.2,1);
points[1] = (1.3,1.6,1.8);
points[2] = (2.9,2.4,2.3);
points[3] = (3.2,3.5,3);
points[4] = (3.6,3.2,4);
points[5] = (4.3,4.3,4.5);

// bbox
triple bboxmin = (0.6,1.2,1);
triple bboxmax = (4.3,4.3,4.5);

// oriented bounding box
triple[] obpts = new triple[8];
obpts[0] = (4.47275,4.77642,4.26472);
obpts[1] = (5.01943,4.16119,4.22572);
obpts[2] = (4.14318,4.44238,4.91437);
obpts[3] = (4.68985,3.82716,4.87537);
obpts[4] = (0.610147,1.57284,0.657968);
obpts[5] = (1.15682,0.95762,0.618965);
obpts[6] = (0.280575,1.23881,1.30761);
obpts[7] = (0.827249,0.623585,1.26861);

// draw points
dot(points[0], blue);
dot(points[1], blue);
dot(points[2], blue);
dot(points[3], blue);
dot(points[4], blue);
dot(points[5], blue);

// draw bbox
draw(box(bboxmin, bboxmax));

// draw oriented bounding box
path3[] obboxpath = obpts[0]--obpts[1]--obpts[3]--obpts[2]--cycle
     ^^ obpts[4]--obpts[5]--obpts[7]--obpts[6]--cycle
     ^^ obpts[0]--obpts[4] ^^ obpts[1]--obpts[5] ^^ obpts[2]--obpts[6] ^^ obpts[3]--obpts[7];
draw(obboxpath, orange);

