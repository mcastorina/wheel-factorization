import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="wheel_factorize",
    version="1.0.1",
    author="miccah",
    author_email="m.castorina93@gmail.com",
    description="Library for finding prime factorizations and primality",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/mcastorina/wheel-factorization",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 2",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Operating System :: OS Independent",
    ],
)
