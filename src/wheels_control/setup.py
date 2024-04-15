from setuptools import find_packages, setup

package_name = 'wheels_control'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', ['launch/wheels_control.launch.py']),
        ('share/ament_index/resource_index/packages', ['resource/' + package_name])
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='lemmy',
    maintainer_email='lemmyhu@gmail.com',
    description='Controlling Dynamixel Motors',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'wheels_control = wheels_control.wheels_control:main'
        ],
    },
)
