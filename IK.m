% Inverse Kinematics for a 3-DOF RRR manipulator with graphical representation
clc; clear; close all;

% Define the parameters
L1 = 185; % Length of first link
L2 = 117.5; % Length of second link
L3 = 80; % Length of third link

% Input: End-effector position (x, y, phi)
x = 200; % Example value
y = 150; % Example value
phi = pi/2; % Example value

% Calculate intermediate variables
c3 = (x^2 + y^2 - L1^2 - L2^2) / (2 * L1 * L2);

% Ensure c3 is within the valid range
if abs(c3) > 1
    error('The given position is unreachable.');
end

s3 = sqrt(1 - c3^2); % Assuming the positive solution
theta3 = atan2(s3, c3);

% Solve for theta2
k1 = L1 + L2 * cos(theta3);
k2 = L2 * sin(theta3);
theta2 = atan2(y, x) - atan2(k2, k1);

% Solve for theta1
theta1 = phi - theta2 - theta3;

fprintf('Inverse Kinematics:\n');
fprintf('theta1: %.2f, theta2: %.2f, theta3: %.2f\n', theta1, theta2, theta3);

% Plot the manipulator
figure;
hold on;
axis equal;
xlim([-3 3]);
ylim([-3 3]);

% Base
plot(0, 0, 'ko', 'MarkerSize', 10, 'MarkerFaceColor', 'k');

% First link
x1 = L1 * cos(theta1);
y1 = L1 * sin(theta1);
plot([0 x1], [0 y1], 'b-', 'LineWidth', 2);
plot(x1, y1, 'bo', 'MarkerSize', 5, 'MarkerFaceColor', 'b');

% Second link
x2 = x1 + L2 * cos(theta1 + theta2);
y2 = y1 + L2 * sin(theta1 + theta2);
plot([x1 x2], [y1 y2], 'r-', 'LineWidth', 2);
plot(x2, y2, 'ro', 'MarkerSize', 5, 'MarkerFaceColor', 'r');

% Third link
x3 = x2 + L3 * cos(theta1 + theta2 + theta3);
y3 = y2 + L3 * sin(theta1 + theta2 + theta3);
plot([x2 x3], [y2 y3], 'g-', 'LineWidth', 2);
plot(x3, y3, 'go', 'MarkerSize', 5, 'MarkerFaceColor', 'g');

title('3-DOF RRR Manipulator');
xlabel('X');
ylabel('Y');
grid on;
