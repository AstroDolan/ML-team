Table table;
float margin = 20;

float minX, maxX, minY, maxY;

void setup() {
  size(800, 800);
  table = loadTable("partition.csv", "header");
  background(255);
  stroke(0);
  strokeWeight(2);
  
  calcularLimites();
  drawPartition();
}

void calcularLimites() {
  minX = Float.MAX_VALUE; maxX = Float.MIN_VALUE;
  minY = Float.MAX_VALUE; maxY = Float.MIN_VALUE;

  for (TableRow row : table.rows()) {
    float x1 = row.getFloat("x1");
    float y1 = row.getFloat("y1");
    float x2 = row.getFloat("x2");
    float y2 = row.getFloat("y2");

    minX = min(minX, min(x1, x2));
    maxX = max(maxX, max(x1, x2));
    minY = min(minY, min(y1, y2));
    maxY = max(maxY, max(y1, y2));
  }
}

void drawPartition() {
  for (TableRow row : table.rows()) {
    float x1 = map(row.getFloat("x1"), minX, maxX, margin, width - margin);
    float y1 = map(row.getFloat("y1"), minY, maxY, height - margin, margin);
    float x2 = map(row.getFloat("x2"), minX, maxX, margin, width - margin);
    float y2 = map(row.getFloat("y2"), minY, maxY, height - margin, margin);
    
    stroke(0);         // negro
    strokeWeight(3);   // más grueso
    line(x1, y1, x2, y2);
  }

  // Opcional: si quieres marcar extremos
  for (TableRow row : table.rows()) {
    float x1 = map(row.getFloat("x1"), minX, maxX, margin, width - margin);
    float y1 = map(row.getFloat("y1"), minY, maxY, height - margin, margin);
    float x2 = map(row.getFloat("x2"), minX, maxX, margin, width - margin);
    float y2 = map(row.getFloat("y2"), minY, maxY, height - margin, margin);

    noStroke();
    fill(255, 0, 0); ellipse(x1, y1, 6, 6);  // punto 1 rojo
    fill(0, 0, 255); ellipse(x2, y2, 6, 6);  // punto 2 azul
  }
}
