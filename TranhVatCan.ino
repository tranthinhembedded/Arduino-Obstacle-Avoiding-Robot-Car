//Jugaad - 101//
//ARDUINO OBSTACLE AVOIDING CAR//
//Install Necessary Libraries//
// Cài đặt các thư viện cần thiết
#include <AFMotor.h> // Thư viện điều khiển động cơ Adafruit Motor Shield
#include <NewPing.h> // Thư viện dùng để điều khiển cảm biến siêu âm
#include <Servo.h> // Thư viện điều khiển servo

// Định nghĩa các chân kết nối cho cảm biến siêu âm
#define TRIG_PIN A0 // Chân TRIG của cảm biến siêu âm kết nối với chân A0 của Arduino
#define ECHO_PIN A1 // Chân ECHO của cảm biến siêu âm kết nối với chân A1 của Arduino
#define MAX_DISTANCE 100 // Khoảng cách tối đa cảm biến có thể đo được (100 cm)
#define MAX_SPEED 190 // Tốc độ tối đa của động cơ
#define MAX_SPEED_OFFSET 20 // Độ bù tốc độ tối đa

// Khởi tạo đối tượng cảm biến siêu âm với các chân và khoảng cách tối đa
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// Khởi tạo các động cơ với các kênh tương ứng và tần số
AF_DCMotor motor1(1, MOTOR12_1KHZ); // Động cơ 1 kết nối với kênh 1
AF_DCMotor motor2(2, MOTOR12_1KHZ); // Động cơ 2 kết nối với kênh 2
AF_DCMotor motor3(3, MOTOR34_1KHZ); // Động cơ 3 kết nối với kênh 3
AF_DCMotor motor4(4, MOTOR34_1KHZ); // Động cơ 4 kết nối với kênh 4
Servo myservo; // Khởi tạo đối tượng servo

boolean goesForward = false; // Biến để kiểm tra xe có đang tiến về phía trước hay không
int distance = 100; // Khoảng cách ban đầu từ cảm biến siêu âm
int speedSet = 0; // Tốc độ thiết lập ban đầu

// Hàm thiết lập ban đầu
void setup() {
  myservo.attach(10); // Gắn servo vào chân số 10
  myservo.write(115); // Điều chỉnh góc của servo
  delay(2000); // Đợi 2 giây
  distance = readPing(); // Đọc khoảng cách từ cảm biến siêu âm
  delay(100); // Đợi 100 ms
  distance = readPing(); // Đọc lại khoảng cách
  delay(100); // Đợi 100 ms
  distance = readPing(); // Đọc lại khoảng cách
  delay(100); // Đợi 100 ms
  distance = readPing(); // Đọc lại khoảng cách
  delay(100); // Đợi 100 ms
}

// Vòng lặp chính
void loop() {
  int distanceR = 0; // Biến lưu khoảng cách bên phải
  int distanceL = 0; // Biến lưu khoảng cách bên trái
  delay(40); // Đợi 40 ms
  if (distance <= 40) { // Nếu khoảng cách <= 15 cm
    moveStop(); // Dừng xe
    delay(100); // Đợi 100 ms
    moveBackward(); // Lùi xe
    delay(300); // Đợi 300 ms
    moveStop(); // Dừng xe
    delay(200); // Đợi 200 ms
    distanceR = lookRight(); // Nhìn sang phải và đọc khoảng cách
    delay(200); // Đợi 200 ms
    distanceL = lookLeft(); // Nhìn sang trái và đọc khoảng cách
    delay(200); // Đợi 200 ms
    if (distanceR >= distanceL) { // Nếu khoảng cách bên phải lớn hơn hoặc bằng bên trái
      turnRight(); // Quẹo phải
      moveStop(); // Dừng xe
    } else { // Nếu khoảng cách bên trái lớn hơn
      turnLeft(); // Quẹo trái
      moveStop(); // Dừng xe
    }
  } else { // Nếu khoảng cách > 15 cm
    moveForward(); // Tiến về phía trước
  }
  distance = readPing(); // Đọc lại khoảng cách từ cảm biến siêu âm
}

// Hàm nhìn sang phải và đọc khoảng cách
int lookRight() {
  myservo.write(50); // Điều chỉnh servo để nhìn sang phải
  delay(500); // Đợi 500 ms
  int distance = readPing(); // Đọc khoảng cách từ cảm biến siêu âm
  delay(100); // Đợi 100 ms
  myservo.write(115); // Điều chỉnh servo về vị trí ban đầu
  return distance; // Trả về khoảng cách đã đọc
}

// Hàm nhìn sang trái và đọc khoảng cách
int lookLeft() {
  myservo.write(170); // Điều chỉnh servo để nhìn sang trái
  delay(500); // Đợi 500 ms
  int distance = readPing(); // Đọc khoảng cách từ cảm biến siêu âm
  delay(100); // Đợi 100 ms
  myservo.write(115); // Điều chỉnh servo về vị trí ban đầu
  return distance; // Trả về khoảng cách đã đọc
  delay(100); // Đợi 100 ms
}

// Hàm đọc khoảng cách từ cảm biến siêu âm
int readPing() {
  delay(70); // Đợi 70 ms
  int cm = sonar.ping_cm(); // Đọc khoảng cách tính bằng cm
  if (cm == 0) { // Nếu không đọc được khoảng cách
    cm = 250; // Gán giá trị khoảng cách là 250 cm (giá trị lớn để báo hiệu không có vật cản)
  }
  return cm; // Trả về khoảng cách đã đọc
}

// Hàm dừng xe
void moveStop() {
  motor1.run(RELEASE); // Dừng động cơ 1
  motor2.run(RELEASE); // Dừng động cơ 2
  motor3.run(RELEASE); // Dừng động cơ 3
  motor4.run(RELEASE); // Dừng động cơ 4
}

// Hàm tiến về phía trước
void moveForward() {
  if (!goesForward) { // Nếu xe chưa tiến về phía trước
    goesForward = true; // Thiết lập xe đang tiến về phía trước
    motor1.run(FORWARD); // Chạy động cơ 1 tiến về phía trước
    motor2.run(FORWARD); // Chạy động cơ 2 tiến về phía trước
    motor3.run(FORWARD); // Chạy động cơ 3 tiến về phía trước
    motor4.run(FORWARD); // Chạy động cơ 4 tiến về phía trước
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) { // Tăng dần tốc độ
      motor1.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 1
      motor2.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 2
      motor3.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 3
      motor4.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 4
      delay(5); // Đợi 5 ms
    }
  }
}

// Hàm lùi xe
void moveBackward() {
  goesForward = false; // Thiết lập xe không tiến về phía trước
  motor1.run(BACKWARD); // Chạy động cơ 1 lùi về phía sau
  motor2.run(BACKWARD); // Chạy động cơ 2 lùi về phía sau
  motor3.run(BACKWARD); // Chạy động cơ 3 lùi về phía sau
  motor4.run(BACKWARD); // Chạy động cơ 4 lùi về phía sau
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) { // Tăng dần tốc độ
    motor1.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 1
    motor2.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 2
    motor3.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 3
    motor4.setSpeed(speedSet); // Thiết lập tốc độ cho động cơ 4
    delay(5); // Đợi 5 ms
  }
}

// Hàm quẹo phải
void turnRight() {
  motor1.run(FORWARD); // Chạy động cơ 1 tiến về phía trước
  motor2.run(FORWARD); // Chạy động cơ 2 tiến về phía trước
  motor3.run(BACKWARD); // Chạy động cơ 3 lùi về phía sau
  motor4.run(BACKWARD); // Chạy động cơ 4 lùi về phía sau
  delay(150); // Đợi 500 ms
  motor1.run(FORWARD); // Chạy động cơ 1 tiến về phía trước
  motor2.run(FORWARD); // Chạy động cơ 2 tiến về phía trước
  motor3.run(FORWARD); // Chạy động cơ 3 tiến về phía trước
  motor4.run(FORWARD); // Chạy động cơ 4 tiến về phía trước
}

// Hàm quẹo trái
void turnLeft() {
  motor1.run(BACKWARD); // Chạy động cơ 1 lùi về phía sau
  motor2.run(BACKWARD); // Chạy động cơ 2 lùi về phía sau
  motor3.run(FORWARD); // Chạy động cơ 3 tiến về phía trước
  motor4.run(FORWARD); // Chạy động cơ 4 tiến về phía trước
  delay(150); // Đợi 500 ms
  motor1.run(FORWARD); // Chạy động cơ 1 tiến về phía trước
  motor2.run(FORWARD); // Chạy động cơ 2 tiến về phía trước
  motor3.run(FORWARD); // Chạy động cơ 3 tiến về phía trước
  motor4.run(FORWARD); // Chạy động cơ 4 tiến về phía trước
}
