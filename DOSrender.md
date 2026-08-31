# DOS shader

## 256色绘制模式

### 初始化

```cpp
Dos256Draw dosDraw;
dosDraw.setMode13h();
dosDraw.setGrayscalePalette();
```

### 每帧清除

```cpp
dosmemput(dosDraw.screen_buffer, 64000, 0xA0000);
```



### 退出

```cpp
/* 恢复文本模式并退出 */
dosDraw.setTextColorMode();
```



## 32位真彩色绘制模式

### 初始化

```cpp
Dos32Draw dosDraw(vec2(320.0f, 200.0f));
//目前只支持320.0,200.0
```

