target = app

# 得到所有.c文件
src = $(wildcard ./*.cpp)

# 将.c替换为.o
obj = $(patsubst ./%.cpp, ./%.o, $(src))

CC = gcc

$(target): $(obj)
	$(CC) -lpthread $(obj) -o $(target)

%.o: %.cpp
	$(CC) -c -lpthread $< -o $@

# 声明伪目标
.PHONY:clean
clean:
	rm $(obj) $(target)
