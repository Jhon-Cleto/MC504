../bin/./matmul 1.in >test1.out
diff -s test1.out 1.out
rm test1.out

../bin/./matmul 2.in >test2.out
diff -s test2.out 2.out
rm test2.out

../bin/./matmul 3.in >test3.out
diff -s test3.out 3.out
rm test3.out

../bin/./matmul 4.in 2> test4.out
diff -s test4.out 4.out
rm test4.out

../bin/./matmul 5.in >test5.out
diff -s test5.out 5.out
rm test5.out

../bin/./matmul 6.in >test6.out
diff -s test6.out 6.out
rm test6.out

../bin/./matmul 7.in >test7.out
diff -s test7.out 7.out
rm test7.out

../bin/./matmul 8.in >test8.out
diff -s test8.out 8.out
rm test8.out

../bin/./matmul 9.in >test9.out
diff -s test9.out 9.out
rm test9.out

../bin/./matmul 10.in >test10.out
diff -s test10.out 10.out
rm test10.out