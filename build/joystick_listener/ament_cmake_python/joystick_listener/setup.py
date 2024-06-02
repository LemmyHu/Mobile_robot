from setuptools import find_packages
from setuptools import setup

setup(
    name='joystick_listener',
    version='0.0.0',
    packages=find_packages(
        include=('joystick_listener', 'joystick_listener.*')),
)
