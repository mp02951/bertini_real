function create_axes(br_plotter)


			curr_axes = gca;

			set(curr_axes,'visible','on');
			set(curr_axes,'FontSize',br_plotter.options.fontsizes.axis);

			hold(curr_axes,'on');



			set(curr_axes,'dataaspectratio',[1 1 1]);

			set(curr_axes,'Tag','bertini_real');



			set(curr_axes,'Position', [0.2 0.2 0.6 0.6]);
			br_plotter.axes.main = curr_axes;

end