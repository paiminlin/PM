import numpy as np
import math
from matplotlib import pyplot
from sklearn.preprocessing import StandardScaler  # 标准化
from sklearn.linear_model import LinearRegression  # 线性回归
from sklearn.model_selection import train_test_split  # 训练集 特征值拆分
from sklearn.metrics import mean_squared_error

# load 数据集
import csv

data_file_name = "LinearRegression.csv"
with open(data_file_name) as f:
    data_file = csv.reader(f)
    temp = next(data_file)
    n_samples = int(temp[0])
    n_features = int(temp[1])
    data = np.empty((n_samples, n_features))
    target = np.empty((n_samples,))
    temp = next(data_file)  # names of features
    feature_names = np.array(temp)

    for i, d in enumerate(data_file):
        data[i] = np.asarray(d[:-1], dtype=int)
        target[i] = np.asarray(d[-1], dtype=int)

# 获取特征值
feature = data  # 特征值
# target
print("data = ", data)
print("target = ", target)

# 分析回归目标值的差异
print("The max target value is ", np.max(target))
print("The min target value is ", np.min(target))
print("The averge target value is ", np.mean(target))

# 进行归一化---目标值是具体的房价，特征值是各个特征，将其转化到一个小范围区间
# 目标值不需要归一化， 特征值需要归一化
stand = StandardScaler()  # 标准化操作
# x_train = stand.fit_transform(data)  # 计算训练数据的均值和方差，基于计算出来的均值和方差来转换训练数据，从而把数据转换成标准的正态分布
# x_test = stand.fit_transform(data)
x_train = data
x_test = data
print("x_train = ", x_train)
print("x_test = ", x_test)

# 进行构建模型 -- 线性模型
peps = LinearRegression()
# 训练数据
peps.fit(x_train, target)
# 使用测试数据进行回归预测
y_predict = peps.predict(x_test)
# 训练数据的预测值
y_train_predict = peps.predict(x_train)
print("y_predict = ", y_predict)
print("y_train_predict = ", y_train_predict)

# 查看训练参数
print(peps.get_params())
# 计算准确率
score = peps.score(x_test, target)
print('准确率:' + r'R^2=%f' % score)
# 系数
print('系数为:\n coef_ = %s \n intercept_ = %s' % (peps.coef_, peps.intercept_))

# 计算MSE(平均方差)
MSE = mean_squared_error(target, y_train_predict)
# 计算RMSE（均方根误差）
RMSE = math.sqrt(MSE)
print('MSE = %.4f' % MSE)
print('RMSE = %.f' % RMSE)

# 画布
pyplot.figure()
# 默认不支持中文，需要配置RC 参数
pyplot.rcParams['font.sans-serif'] = 'SimHei'
# 设置字体之后不支持负号，需要去设置RC参数更改编码
pyplot.rcParams['axes.unicode_minus'] = False
# 绘图
# 折线图
x = np.arange(0, len(y_predict))
pyplot.plot(x, target, marker='*')
pyplot.plot(x, y_predict, marker='.')
# 增加标题
pyplot.title('信号强度与定位的关系')
# 增加横轴，纵轴名称
pyplot.xlabel('x')
pyplot.ylabel('定位')
# 图例
pyplot.legend(['真实值', '预测值'])
# 画图
pyplot.show()
