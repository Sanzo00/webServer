target = app

# 得到所有.c文件
src = $(wildcard ./*.cpp)

# 将.c替换为.o
obj = $(patsubst ./%.cpp, ./%.o, $(src))

CC = g++

$(target): $(obj)
	$(CC) $(obj) -o $(target) -lpthread

%.o: %.cpp
	$(CC) -c $< -o $@ -lpthread

# 声明伪目标
.PHONY:clean
clean:
	rm $(obj) $(target)
