const fs = require("node:fs");
const readline = require("node:readline");

let content = "";
//create 2d map
function create2DMap(width, height, defaultValue) {
  const map = new Array(height); // Create an outer array of height 96

  for (let y = 0; y < height; y++) {
    map[y] = new Array(width); // Create an inner array for each row
    for (let x = 0; x < width; x++) {
      map[y][x] = defaultValue; // Initialize all locations with defaultValue
    }
  }

  return map;
}

let map = create2DMap(96, 96, " ");

const file = readline.createInterface({
  input: fs.createReadStream("lidar"),
});

file.on("line", (line) => {
  const tokens = line.split("@");
  const distance = parseFloat(tokens[0]);
  const angle = parseFloat(tokens[1]);
  const radians = (angle * Math.PI) / 180;
  let x = Math.floor(distance * Math.cos(radians));
  let y = Math.floor(distance * Math.sin(radians));
  //check for bounds and negative values
  x < 0 ? (x *= -1) : x;
  y < 0 ? (y *= -1) : y;

  if (x >= 0 && x < 96 && y >= 0 && y < 96) {
    // console.log(`mapping: (${x}, ${y}) `);
    map[y][x] = "#";
  } else {
    console.log(`Out of bounds (${x},${y}) `);
  }
});

//iterate through 2d map and print to console
file.on("close", () => {
  for (let i = 0; i < 96; i++) {
    for (let j = 0; j < 96; j++) {
      process.stdout.write(map[i][j]);
    }
    process.stdout.write("\n");
  }
});
//print map to file

// fs.writeFile("map", map, (err) => {
//   if (err) throw err;
//   console.log("The file has been saved!");
// });
