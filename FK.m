% Forward Kinematics for a 3-DOF RRR manipulator with graphical representation
clc; clear; close all;

% Define the parameters
L1 = 185; % Length of first link
L2 = 117.5; % Length of second link
L3 = 80; % Length of third link

% Input: Joint angles theta1, theta2, theta3
theta1 = pi/4; % Example value
theta2 = pi/4; % Example value
theta3 = pi/4; % Example value

% Compute the position of the end-effector
x = L1 * cos(theta1) + L2 * cos(theta1 + theta2) + L3 * cos(theta1 + theta2 + theta3);
y = L1 * sin(theta1) + L2 * sin(theta1 + theta2) + L3 * sin(theta1 + theta2 + theta3);
phi = theta1 + theta2 + theta3;

fprintf('Forward Kinematics:\n');
fprintf('x: %.2f, y: %.2f, phi: %.2f\n', x, y, phi);

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
